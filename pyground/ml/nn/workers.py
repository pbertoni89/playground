from multiprocessing import Process, Queue, Pipe
import numpy as np
import os
import time
import logging
import signal

import marcore

# to remove dependency, downgrade our xslogging wrapper to python default logging
import sys
sys.path.insert(0, '/home/xnext/xspectra-dev/xspyctra')
from mngr import xslogging

_lvl = logging.INFO


def res_sum_squares(y_true, y_pred):
    import keras.backend
    squared_diff = (y_true - y_pred) ** 2
    return keras.backend.sum(squared_diff)


def load_model(model_path):
    import keras.models
    print("loading model {}".format(model_path))
    time_start = time.time()
    model = keras.models.load_model(model_path, compile=True, custom_objects={'res_sum_squares': res_sum_squares})
    print("model loaded in {:.2f} s".format(time.time() - time_start))
    return model

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


class Worker(Process):
    def __init__(self, puid, queue, pipe):
        Process.__init__(self, name='ModelProcessor')
        self._puid = puid
        self._queue = queue
        self._pipe = pipe
        self._logger = xslogging.factory("worker {}".format(puid), level=_lvl)
        self.predictions, self.runtime, self.predtime = 0, 0, 0
        self._stop = False

    def catch_sigterm(self, foo, _):
        self._logger.error('sigterm catched {} from {}'.format(foo, self.pid))
        self._stop = True

    def __load_model(self, model_path):
        import keras.models
        time_start = time.time()
        model = keras.models.load_model(model_path, compile=True, custom_objects={'res_sum_squares': res_sum_squares})
        self._logger.info('model {} loaded in {:.2f} s'.format(model_path, time.time() - time_start))
        return model

    def run(self):
        """
            Implements abstract Process.run().
            Load NN, then start consumption
        """
        self._logger.info('run() start. Pid {}. Queue size {}'.format(self.pid, self._queue.qsize()))
        # only HERE the Process has it's own scope
        signal.signal(signal.SIGTERM, self.catch_sigterm)

        model = self.__load_model('model.h5')

        # empty queue check COULD be wrong, so let's ensure it happens more than once.
        empty_sleep, empty_dead = 0.01, 1
        # ROUGH control: when counter reaches condition, Queue is really considered dead (stop still applies)
        empty_cnt, empty_condition = 0, int(empty_dead / empty_sleep)
        time_start = time.time()

        while empty_cnt < empty_condition or self._stop is False:

            col = self._queue.get()

            if col is None:
                self._queue.put(None)
                self._logger.debug('queue is empty {} < {}. Stop {}'.format(empty_cnt, empty_condition, self._stop))
                empty_cnt += 1
                time.sleep(empty_sleep)
            else:
                empty_cnt = 0
                self._logger.debug('queue size {}. Stop {}'.format(self._queue.qsize(), self._stop))
                _ = self.predict(model, col)

        self.runtime = time.time() - time_start
        self.predtime = self.runtime / self.predictions
        self._logger.info('run() end. Consumption of {} columns took {:.2f} s -> {:.2f} ms/col'
                          .format(self.predictions, self.runtime, self.predtime * 1000))
        self._pipe.send((self._puid, self.runtime, self.predictions))
        self._pipe.close()

    def predict(self, model, col):
        # Critical Call
        rv = marcore.denoise(model=model, col=col)
        self.predictions += 1
        self._logger.debug('predict() # {}: {:.2f} -> {:.2f}'.format(self.predictions, np.mean(col), np.mean(rv)))
        return rv


class WorkerGpu(Worker):
    def __init__(self, puid, queue, pipe):
        super(WorkerGpu, self).__init__(puid, queue, pipe)

    def run(self):
        os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
        os.environ["CUDA_VISIBLE_DEVICES"] = str(self._puid)

        super(WorkerGpu, self).run()


class WorkerCpu(Worker):
    def __init__(self, puid, queue, pipe):
        super(WorkerCpu, self).__init__(puid, queue, pipe)

    def run(self):
        import tensorflow as tf
        import keras.backend
        # from tensorflow.python.client import device_lib

        # print(device_lib.list_local_devices())
        self._logger.info('setting CPU environment')
        config = tf.ConfigProto(device_count={'GPU': 0, 'CPU': 1},
                                intra_op_parallelism_threads=1,
                                inter_op_parallelism_threads=8,
                                # choose an existing and supported device to run, in case the specified doesn't exist
                                allow_soft_placement=True,
                                # stdout additional data
                                log_device_placement=True)
        session = tf.Session(config=config)
        keras.backend.set_session(session)
        self._logger.info('CPU environment set')

        # print(device_lib.list_local_devices())

        super(WorkerCpu, self).run()


class Scheduler:
    def __init__(self, l_puids):
        """
            Initialize workers based on their Processing Unit Ids
            :param l_puids: list of ids
        """
        self._queue = Queue()
        self._logger = xslogging.factory("scheduler")

        self._lt_puids_pipes = []
        for puid in l_puids:
            (parent_conn, child_conn) = Pipe()
            wkr = WorkerGpu(puid, self._queue, child_conn) if puid >= 0 else WorkerCpu(puid, self._queue, child_conn)
            self._lt_puids_pipes.append((puid, wkr, parent_conn))

    def produce(self, frame):
        """
            Push to Queue a new list of columns
            :param frame: the list
        """
        sz_before = self._queue.qsize()
        # Production - put all of files into queue
        for col in frame:
            self._queue.put(col)
        # add a None into queue to indicate the end of task
        self._queue.put(None)
        self._logger.info('accepted production of {} new columns. Queue size {} -> {}'.format(
                          len(frame), sz_before, self._queue.qsize()))

    def receive(self):
        """
            :return: todo how could the denoised columns be achieved
        """
        pass

    def start(self):
        """
            Trigger the Consumption 'Start' event. Wait for 5 seconds to let init completions
        """
        self._logger.warn('starting workers')
        for _, sv, _ in self._lt_puids_pipes:
            sv.start()
        time.sleep(5)

    def stop(self):
        """
            Send Stop event and wait for all workers to finish job. Print statistics
        """
        predictions = 0
        l_time_el = []

        self._logger.warn('stopping workers')
        for _, sv, _ in self._lt_puids_pipes:
            self._logger.debug('sending sigterm to {}'.format(sv.pid))
            sv.terminate()

        # Blocks until worker writes to Pipe
        self._logger.warn('fetching results from workers')
        for puid, wrk, parent_conn in self._lt_puids_pipes:
            (rpuid, rruntime, rprediction) = parent_conn.recv()
            assert(puid == rpuid)
            predictions += rprediction
            l_time_el.append(rruntime)
            wrk.join()

        mean_time_el = np.mean(l_time_el)
        assert(mean_time_el > 0)
        self._logger.info('final stats: {} prediction in {:.2f} s mean time -> {:.2f} ms/column'.
                          format(predictions, mean_time_el, int(1000 * mean_time_el / predictions)))
