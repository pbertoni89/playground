"""
    https://www.learnopencv.com/blob-detection-using-opencv-python-c/
"""

# Standard imports
from builtins import bool

import cv2 as cv
import numpy as np


def simple_blob(im):
    params = cv.SimpleBlobDetector_Params()
    # Change thresholds - they're values inside [0, 255] range
    params.minThreshold = 50
    params.maxThreshold = 120
    # Filter by Area: brick is about 256 x 80 ~= 25000
    params.filterByArea = True
    params.minArea = 1000
    # Filter by Circularity
    params.filterByCircularity = False
    # params.minCircularity = 0.1
    # Filter by Convexity
    params.filterByConvexity = False
    # params.minConvexity = 0.20
    # Filter by Inertia
    params.filterByInertia = False
    # params.minInertiaRatio = 0.01

    # Create a detector with the parameters
    detector = cv.SimpleBlobDetector_create(params)

    # Detect blobs.
    keypoints = detector.detect(im)

    # Draw detected blobs as red circles.
    # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob

    rgb = (0, 0, 255)
    im_with_keypoints = cv.drawKeypoints(im, keypoints, np.array([]), rgb, cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    _, im_tsmin = cv.threshold(im, params.minThreshold, 255, cv.THRESH_BINARY)
    _, im_tsmax = cv.threshold(im, params.maxThreshold, 255, cv.THRESH_BINARY)
    cv.imshow("Keypoints", im_with_keypoints)
    cv.imshow("ts min", im_tsmin)
    cv.imshow('ts max', im_tsmax)


def boh(im):
    """
        https://www.bluetin.io/opencv/object-detection-tracking-opencv-python/
        :param im: the image
        :return:
    """
    _, mask = cv.threshold(im, 150, 255, cv.THRESH_BINARY)

    # third parameter, contour approximation method, will collect only the endpoint coordinates of straight lines
    im, contours, hierarchy = cv.findContours(mask, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

    # finding The Largest Contour
    contour_sizes = [(cv.contourArea(contour), contour) for contour in contours]
    biggest_contour = max(contour_sizes, key=lambda v: v[0])[1]
    x, y, w, h = cv.boundingRect(biggest_contour)
    im = cv.rectangle(im, (x, y), (x + w, y + h), (0, 255, 0), 2)
    cv.imshow('Contours', im)


def otzu(im):
    # global thresholding
    ret1, th1 = cv.threshold(im, 127, 255, cv.THRESH_BINARY)
    # Otsu's thresholding
    ret2, th2 = cv.threshold(im, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU)
    # Otsu's thresholding after Gaussian filtering
    blur = cv.GaussianBlur(im, (5, 5), 0)
    ret3, th3 = cv.threshold(blur, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU)
    return th3


if __name__ == '__main__':
    # Read image
    fn = '/media/xnext/DATA/logging/blobs.png'
    _im = cv.imread(fn, cv.IMREAD_GRAYSCALE)

    # revert 4x magnification
    _im = cv.resize(src=_im, dsize=None, fx=.25, fy=.25, interpolation=cv.INTER_NEAREST)  # type: np.ndarray

    # Show
    cv.imshow("Source", _im)
    cv.imshow('Otzu TS', otzu(_im))
    simple_blob(_im.copy())
    boh(_im.copy())
    cv.waitKey(0)
