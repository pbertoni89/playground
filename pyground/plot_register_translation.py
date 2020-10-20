"""
=====================================
Cross-Correlation (Phase Correlation)
=====================================

In this example, we use phase correlation to identify the relative shift
between two similar-sized images.

The ``register_translation`` function uses cross-correlation in Fourier space,
optionally employing an upsampled matrix-multiplication DFT to achieve
arbitrary subpixel precision. [1]_

.. [1] Manuel Guizar-Sicairos, Samuel T. Thurman, and James R. Fienup,
       "Efficient subpixel image registration algorithms," Optics Letters 33,
       156-158 (2008).

"""
import numpy as np
import matplotlib.pyplot as plt

from skimage import data
from skimage.feature import register_translation
from skimage.feature.register_translation import _upsampled_dft
from scipy.ndimage.fourier import fourier_shift

image = data.camera()
print('image size: {} w x {} h'.format(*image.shape))
# aprio_shift = (-2.4, 1.32)  # pixel offset relative to reference coin
aprio_shift = (-54.56, 38.2)  # custom edit
offset_image = fourier_shift(np.fft.fftn(image), aprio_shift)
offset_image = np.fft.ifftn(offset_image)
image_product = np.fft.fft2(image) * np.fft.fft2(offset_image).conj()
print("Known offset (y, x):")
print(aprio_shift)


def precision_pixel():
    shift, error, diffphase = register_translation(image, offset_image)

    fig, (ax1, ax2, ax3) = plt.subplots(ncols=3, figsize=(8, 3))

    ax1.imshow(image)
    ax1.set_axis_off()
    ax1.set_title('Reference image')

    ax2.imshow(offset_image.real)
    ax2.set_axis_off()
    ax2.set_title('Offset image')

    # View the output of a cross-correlation to show what the algorithm is
    #    doing behind the scenes
    cc_image = np.fft.fftshift(np.fft.ifft2(image_product))
    ax3.imshow(cc_image.real)
    ax3.set_axis_off()
    ax3.set_title("Cross-correlation")

    print("Detected pixel offset (y, x):")
    print(shift)
    plt.show()


def precision_subpixel():
    shift, error, diffphase = register_translation(image, offset_image, 100)

    fig, (ax1, ax2, ax3) = plt.subplots(ncols=3, figsize=(8, 3))

    ax1.imshow(image)
    ax1.set_axis_off()
    ax1.set_title('Reference image')

    ax2.imshow(offset_image.real)
    ax2.set_axis_off()
    ax2.set_title('Offset image')

    # Calculate the upsampled DFT, again to show what the algorithm is doing
    #    behind the scenes.  Constants correspond to calculated values in routine.
    #    See source code for details.
    cc_image = _upsampled_dft(image_product, 150, 100, (shift*100)+75).conj()
    ax3.imshow(cc_image.real)
    ax3.set_axis_off()
    ax3.set_title("Supersampled XC sub-area")

    print("Detected subpixel offset (y, x):")
    print(shift)
    plt.show()


if __name__ == '__main__':
    precision_pixel()
    precision_subpixel()
