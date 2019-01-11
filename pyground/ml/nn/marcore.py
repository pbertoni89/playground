import numpy as np
from matplotlib import pyplot as plt


npixels, nbins = 128, 128
col_counts = npixels * nbins
col_bytes = col_counts * np.dtype(np.uint16).itemsize
__magic = 256
__shape_col = (npixels, nbins)
__shape_nn = (1, npixels, nbins, 1)


def plot_alg(col, col_eq, col_eq_norm, col_den):
    plt.subplot(2, 2, 1)
    plt.plot(np.transpose(col))
    plt.title("source")
    plt.subplot(2, 2, 2)
    plt.plot(np.transpose(col_eq))
    plt.title("equalized")
    plt.subplot(2, 2, 3)
    plt.plot(np.transpose(col_eq_norm))
    plt.title("equalized normalized")
    plt.subplot(2, 2, 4)
    plt.plot(np.transpose(col_den))
    plt.title("denoised")
    plt.show()


def _hist_equalization(col):
    """
        :param col: the source column
        :return: the equalized column
    """
    # flat the column in one dimension, histogram with `magic` bins
    hist, _ = np.histogram(col.flatten(), __magic, [0, __magic - 1])
    cdf = hist.cumsum()
    cdf_min = np.amin(np.ma.masked_equal(cdf, 0))
    cdf_max = np.amax(cdf)
    h = (cdf - cdf_min).astype(np.float) / (cdf_max - cdf_min) * (__magic - 1)
    # cast to uint
    h = h.astype(np.uint16)
    return h[col]


def denoise(model, col, ratio=None, plot=False):
    """
        Marco Re A.P.I

        :param model: the NN model

        :param col: the source column

        :param ratio: logging purposes

        :param plot: enable matplotlib

        :return: the denoised column
    """
    # Equalize and Normalize
    col_eq = _hist_equalization(col)
    col_eq_norm = np.divide(col_eq.astype('float'), __magic - 1)
    # Denoise
    tmp_denoise = np.reshape(col_eq_norm, __shape_nn)

    tmp_denoise = model.predict(tmp_denoise)

    if ratio is not None and ratio % 1 == 0:
        print("\r{} %".format(ratio), end="")
    col_denoised = np.reshape(tmp_denoise, __shape_col)
    # pare sia corretto rimoltiplicare per il fattore magico
    col_denoised = col_denoised * __magic
    col_denoised = col_denoised.astype(np.uint16)
    if plot:
        plot_alg(col, col_eq, col_eq_norm, col_denoised)
    return col_denoised
