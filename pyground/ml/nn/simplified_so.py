import time
import keras
import numpy as np
import threading
import pathos.multiprocessing
import multiprocessing


def __res_sum_squares(y_true, y_pred):
    squared_diff = (y_true - y_pred) ** 2
    return keras.backend.sum(squared_diff)


__npixels, __nbins = 128, 128

"""
    Context: a "cube" is a 3D ndarray. Each vertical slice is a "column", "npixels" heigth, "nbins" depth

    Each prediction transforms a column (i.e. a 128*128 ndarray of integers) in a denoised column
"""

__shape_col = (__npixels, __nbins)
__shape_nn = (1, __npixels, __nbins, 1)
# get model file at https://drive.google.com/file/d/1f0WGCv11uObPziySE2wl6hXYKfyjqXBQ/view?usp=sharing
__model = keras.models.load_model('./model.h5', compile=True, custom_objects={'res_sum_squares': __res_sum_squares})

__max_parallel_predictions = 4
__sema = threading.BoundedSemaphore(value=__max_parallel_predictions)


def __mt_pathos_manager(col_ratio):
    return __denoise(col_ratio[0], col_ratio[1])


def __denoise_frame_mt_pathos(frame_ratios):
    results = pathos.multiprocessing.ProcessingPool().map(__mt_pathos_manager, frame_ratios)
    return results


def __denoise_frame_mt_multiprocessing(frame_ratios):
    pool = multiprocessing.Pool()
    results = pool.map(__denoise, map(lambda col_ratio: col_ratio, frame_ratios))
    pool.close()
    pool.join()
    return results


def __denoise(col, ratio=None):
    """
        :param col: the source column
        :param ratio: logging purposes
        :return: the denoised column
    """
    really_predict = False
    if type(col) is tuple:
        col, ratio = col[0], col[1]
    col_denoise = np.reshape(col, __shape_nn)

    print("{} acquiring".format(ratio))
    __sema.acquire()
    print("{} acquired".format(ratio))
    time_start = time.time()
    #  ~    ~  ~  ~  ~  ~  ~  ~  ~  ~ CRITICAL SECTION START ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
    col_denoise = __model.predict(col_denoise) if really_predict else col_denoise
    #  ~    ~  ~  ~  ~  ~  ~  ~  ~  ~ CRITICAL SECTION END   ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
    print("{} releasing after {} s".format(ratio, time.time() - time_start))
    print("{} releasing".format(ratio))
    __sema.release()
    print("{} released".format(ratio))

    return np.reshape(col_denoise, __shape_col)


def denoise_cube(cube, mp=False, mp_pathos=False):
    """
        :param cube: a numpy 3D array of ncols * npixels * nbins
        :param mp: use multiprocessing
        :param mp_pathos: use pathos multiprocessing
        :return: the denoised cube
    """
    ncol = cube.shape[0]
    ratios = [(ic * 100.0) / ncol for ic in range(0, ncol)]
    frame_ratios = zip(cube, ratios)

    if mp:
        if mp_pathos:
            l_cols_denoised = __denoise_frame_mt_pathos(frame_ratios)
        else:
            l_cols_denoised = __denoise_frame_mt_multiprocessing(frame_ratios)
    else:
        l_cols_denoised = [__denoise(col, ratio) for col, ratio in frame_ratios]
    return l_cols_denoised


if __name__ == "__main__":

    test_cube = np.random.rand(1000, __npixels, __nbins)

    # Single threaded impl: works fine
    # denoise_cube(test_cube, mp=False)
    # Multiprocessing Pool: blocks at the eighth "acquired" print
    # denoise_cube(test_cube, mp=True, mp_pathos=False)
    # Pathos multiprocessing Pool: blocks at the eighth "acquired" print
    denoise_cube(test_cube, mp=True, mp_pathos=True)
