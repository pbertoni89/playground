from multiprocessing import Queue, Process
import time
import argparse

import cv2
import numpy as np
import os
import logging
import signal
from mngr import xslogging

# logging.DEBUG is more verbose
_lvl = logging.INFO


def vgg16(weights_path=None):
    """
        Load a quite fake ANN
        :param weights_path: model file path
        :return:
    """
    from keras.models import Sequential
    from keras.layers.core import Flatten, Dense, Dropout
    from keras.layers.convolutional import Conv2D, MaxPooling2D, ZeroPadding2D
    from keras.optimizers import SGD

    model = Sequential()
    model.add(ZeroPadding2D((1, 1), input_shape=(224, 224, 3)))
    model.add(Conv2D(64, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(64, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D((2, 2), strides=(2, 2)))

    model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D((2, 2), strides=(2, 2)))

    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D((2, 2), strides=(2, 2)))

    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D((2, 2), strides=(2, 2)))

    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D((2, 2), strides=(2, 2)))

    model.add(Flatten())
    model.add(Dense(4096, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(4096, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(1000, activation='softmax'))

    if weights_path:
        try:
            model.load_weights(weights_path)
        except IOError:
            print("{} path not found. Who cares, it's just a demo".format(weights_path))

    sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True)
    model.compile(optimizer=sgd, loss='categorical_crossentropy')

    return model

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


class Worker(Process):
    def __init__(self, gpuid, queue, model=None):
        Process.__init__(self, name='ModelProcessor')
        self._gpuid = gpuid
        self._queue = queue
        self.model = model
        self._logger = xslogging.factory("worker {}".format(gpuid), level=_lvl)
        self._predictions = 0
        self._stop = False
        self._logger.debug("initialized")

    def catch_sigterm(self, foo, _):
        self._logger.error('sigterm catched {} from {}'.format(foo, self.pid))
        self._stop = True

    def _load_model(self):
        if self.model is None:
            self._logger.info('model is being built')
            mod = vgg16('vgg16.h5')
            return mod
        else:
            return self.model

    def run(self):
        """
            Implements abstract Process.run().
            Load NN, then start consumption
        """
        self._logger.info('run() start. Pid {}. Queue size {}'.format(self.pid, self._queue.qsize()))
        # only HERE the Process has it's own scope
        signal.signal(signal.SIGTERM, self.catch_sigterm)

        self.model = self._load_model()
        empty_queue = False
        none_printed = False

        time_start = time.time()
        while empty_queue is False or self._stop is False:

            imgfile = self._queue.get()

            if imgfile is None:
                empty_queue = True
                self._queue.put(None)  # mutex with Queue.qsize() check. Same stuff
                if not none_printed:
                    none_printed = True
                    self._logger.debug('queue is empty. Stop {}'.format(self._stop))
                time.sleep(0.01)
            else:
                empty_queue = False
                none_printed = False
                self._logger.debug('queue size {}. Stop {}'.format(self._queue.qsize(), self._stop))
                _ = self.predict(imgfile)

        time_el = time.time() - time_start
        time_per_pred = time_el / self._predictions
        self._logger.info('run() end. Consumption of {} images took {:.2f} s -> {:.2f} s/image'
                          .format(self._predictions, time_el, time_per_pred))

    def predict(self, imgfile):
        # BGR
        im = cv2.resize(cv2.imread(imgfile), (224, 224)).astype(np.float32)
        # magic
        im[:, :, 0] -= 103.939
        im[:, :, 1] -= 116.779
        im[:, :, 2] -= 123.68

        im = im.reshape((1, 224, 224, 3))
        out = self.model.predict(im)
        rv = np.argmax(out)

        self._predictions += 1
        self._logger.debug('predict() # {}:  {} -> {}'.format(self._predictions, imgfile, rv))
        return rv


class WorkerGpu(Worker):
    def __init__(self, gpuid, queue, model):
        super(WorkerGpu, self).__init__(gpuid, queue, model)

    def run(self):
        """
            Set CUDA environment first, then call Worker.run()
        """
        os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
        os.environ["CUDA_VISIBLE_DEVICES"] = str(self._gpuid)
        self._logger.debug("GPU environment set")
        super(WorkerGpu, self).run()


class WorkerCpu(Worker):
    def __init__(self, gpuid, queue, model):
        super(WorkerCpu, self).__init__(gpuid, queue, model)

    def run(self):
        import tensorflow as tf
        import keras.backend
        # from tensorflow.python.client import device_lib

        # print(" *  *  *  BEFORE S  *  *  *\n{}\n  *  *  *  BEFORE E  *  *  *".format(device_lib.list_local_devices()))
        self._logger.debug('setting CPU environment')
        config = tf.ConfigProto(device_count={'GPU': 0, 'CPU': 1},
                                intra_op_parallelism_threads=1,
                                inter_op_parallelism_threads=8,
                                allow_soft_placement=True,
                                log_device_placement=True)
        session = tf.Session(config=config)
        keras.backend.set_session(session)
        self._logger.debug('CPU environment set')

        # print(" *  *  *  AFTER S  *  *  *\n{}\n  *  *  *  AFTER E  *  *  *".format(device_lib.list_local_devices()))

        super(WorkerCpu, self).run()

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


class Scheduler:
    def __init__(self, l_gpuids):
        self._queue = Queue()
        self._gpuids = l_gpuids
        self._logger = xslogging.factory("scheduler")
        self._workers = list()
        self._init_workers()

    def _factory(self, gpuid, mod, constructor):
        return constructor(gpuid, self._queue, mod)

    def _init_workers(self):
        self._logger.info("_init_workers")

        for gpuid in self._gpuids:
            wkr = self._factory(gpuid, None, WorkerGpu if gpuid >= 0 else WorkerCpu)
            self._workers.append(wkr)

    def produce(self, xfilelst):
        sz_before = self._queue.qsize()
        # Production - put all of files into queue
        for xfile in xfilelst:
            self._queue.put(xfile)
        # add a None into queue to indicate the end of task
        self._queue.put(None)
        self._logger.info('accepted production of {} elements. Queue size {} -> {}'.format(
                          len(xfilelst), sz_before, self._queue.qsize()))

    def start(self):
        """
            Trigger the Consumption 'Start' event. Wait for 5 seconds to let init completions
        """
        self._logger.warn('starting workers')
        for sv in self._workers:
            sv.start()
        time.sleep(5)

    def stop(self):
        """
            Send Stop event and wait for all workers to finish job
        """
        self._logger.warn('stopping workers')
        for sv in self._workers:
            self._logger.debug('sending sigterm to {}'.format(sv.pid))
            sv.terminate()

        self._logger.warn('joining workers')
        for sv in self._workers:
            sv.join()

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


def run(img_path, l_gpuids):
    # scan all files under img_path
    xlist = list()
    for xfile in os.listdir(img_path):
        xlist.append(os.path.join(img_path, xfile))

    # init scheduler
    sc = Scheduler(l_gpuids)
    sc.start()

    time_start = time.time()

    n_bursts = 4
    for irep in range(0, n_bursts):
        sc.produce(xlist)
        # time.sleep(3 if irep else 40)

    # will block for join
    sc.stop()

    time_elapsed = time.time() - time_start
    n_images_tot = n_bursts * len(xlist)
    time_per_image = time_elapsed / n_images_tot

    print("#   *   #   *   #  *   #  *   #  *   #  *   #  *   #  *   #  *   #  *   #  *   #")
    print("MTC is {:.2f} s for {} images -> {:.2f} s/image ".format(time_elapsed, n_images_tot, time_per_image))

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("--imgpath", help="path to your images to be proceed")
    parser.add_argument("--gpuids", help="gpu ids to run")

    args = parser.parse_args()

    gpuids = [int(x) for x in args.gpuids.strip().split(',')]

    run(args.imgpath, gpuids)
