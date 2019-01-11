#!/usr/bin/env python3

import keras
import tensorflow as tf
from tensorflow.python.client import device_lib

import workers
import marcore

# to remove dependency, downgrade our xslogging wrapper to python default logging
import sys
sys.path.insert(0, '/home/xnext/xspectra-dev/xspyctra')
from mngr import xslogging

import numpy as np
import time
import os


def print_tf_keras_env():
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")
    print(device_lib.list_local_devices())
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")
    _ = tf.Session(config=tf.ConfigProto(log_device_placement=True))
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")
    config = tf.ConfigProto(device_count={'GPU': 1, 'CPU': 8})
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")
    sess = tf.Session(config=config)
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")
    keras.backend.set_session(sess)
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")
    print('Default GPU Device: {}'.format(tf.test.gpu_device_name()))
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")
    print("Built with{} Cuda".format("" if tf.test.is_built_with_cuda() else "out"))
    print("  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~")


def __multix_clean_header(fname_src, fname_dst):
    print("cleaning Multix file: {} -> {}".format(fname_src, fname_dst))
    with open(fname_src, "rb") as fr:
        with open(fname_dst, "wb") as fw:
            # throw away header
            _ = fr.read(512)
            col = fr.read(marcore.col_bytes)
            while col != "":
                fw.write(col)
                col = fr.read(1)


def __multix_load_cube(fname):
    """
        if file has header (extra 512 bytes at the beginning) a raw version will be created/used
        :param fname:
        :return: a ncolumns * 128 * 128 ndarray
    """
    raw_size = os.stat(fname).st_size
    ncol_f = raw_size / (marcore.col_bytes * 1.0)
    ncol = np.floor(ncol_f)
    if ncol != ncol_f:
        print("Columns are not rounded: looking/caching a Raw file instead")
        fnameraw = os.path.dirname(fname) + "raw_" + os.path.split(fname)[-1]
        if not os.path.exists(fnameraw):
            __multix_clean_header(fname, fnameraw)
        return __multix_load_cube(fnameraw)
    else:
        ncol = int(ncol)
        file_counts = marcore.col_counts * ncol
        cube = np.fromfile(fname, dtype=np.uint16, count=file_counts)
        cube = np.reshape(cube, (ncol, marcore.npixels, marcore.nbins))
        return cube


def denoise_frame(frame, in_path="", out_path="", model=None):
    """
        :param frame: a numpy 3D array of ncols * npixels * nbins, or None
        :param in_path: if frame_spe is None, read that file
        :param out_path: if not blank, file path for output dump
        :param model: experimental
        :return: the denoised frame_spe
    """
    if frame is None:
        if len(in_path):
            frame = __multix_load_cube(in_path)
        else:
            raise ValueError("must provide a np object or a frame_spe filename")

    time_start = time.time()

    ncol = frame.shape[0]
    ratios = [(ic * 100.0) / ncol for ic in range(0, ncol)]
    frame_ratios = zip(frame, ratios)

    if model is None:
        l_cols_denoised = __sched.produce(frame)
        # sleep is interlaced with Worker activity
        time.sleep(0.5)
    else:
        __logger.info("scheduling frame of {} columns...".format(frame.shape[0]))
        l_cols_denoised = [marcore.denoise(model, col, ratio) for col, ratio in frame_ratios]
        __mean_elapsed_time.append(time.time() - time_start)
        __logger.info("frame took {:.2f} s".format(__mean_elapsed_time[-1]))

    if len(out_path):
        with open(out_path, "wb") as fp:
            [fp.write(col.tobytes()) for col in l_cols_denoised]

#  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~


__sched = workers.Scheduler(l_puids=[-1, 0])
__mean_elapsed_time = []
__logger = xslogging.factory("denoiser")


def test_case(model, _bricks, label):
    global __mean_elapsed_time
    __mean_elapsed_time = []
    save_output = False

    # _bricks = [_bricks[0]]  # todo remove

    for ib, _brick in _bricks:
        _out_path = "./brick_{}.bin".format(ib) if save_output else ""
        denoise_frame(_brick, out_path=_out_path, model=model)

    if model is not None:
        __logger.info("{} approach MET: {:.2f} seconds".format(label, np.mean(__mean_elapsed_time)))


if __name__ == "__main__":

    frame_test = __multix_load_cube("rep7.bin")

    # todo adapt to file == brick scenario
    cols_per_brick = 128
    splitting_range = range(0, frame_test.shape[0], cols_per_brick)
    bricks = [(_ib, frame_test[_ib:_ib + 128, :, :]) for _ib in splitting_range]

    # these two calls BREAK: the first will leave some mess around, ending up with the second complaining about
    # could not retrieve CUDA device count: CUDA_ERROR_NOT_INITIALIZED

    # test_case(workers.load_model('model.h5'), bricks, "Old")

    # print('\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n')

    __sched.start()

    test_case(None, bricks, "New")

    __sched.stop()
