#!/bin/env

# import random
import numpy
from matplotlib import pyplot as plt

from numpy import genfromtxt

# s1 = [random.gauss(3, 1) for _ in range(400)]
# s2 = [random.gauss(4, 2) for _ in range(400)]

dir_log = '/media/xnext/DATA/logging/'
s1 = genfromtxt(dir_log + 'column_bunch' + '.csv', delimiter=',')
s2 = genfromtxt(dir_log + 'column_element' + '.csv', delimiter=',')

bins = numpy.linspace(1000, 4000, 100)

plt.hist(s1, bins, alpha=0.5, label='s1')
plt.hist(s2, bins, alpha=0.5, label='s2')
plt.legend(loc='upper right')
plt.show()
