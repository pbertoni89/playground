import numpy
import pylab
from matplotlib import pyplot as plt
from scipy.optimize import curve_fit

"""
	https://stackoverflow.com/questions/35990467/fit-two-gaussians-to-a-histogram-from-one-set-of-data-python/35992526
"""

samplesP, samplesC = 1000, 1000
aP, aC = 200, 300      # the y coordinate for gaussian peak
muP, muC = 400, 600    # the x coordinate for gaussian peak
sigmaP, sigmaC = 60, 80
maxval = 1000


def gauss(x, mu, sigma, A):
	return A * numpy.exp(-(x - mu) ** 2 / 2 / sigma ** 2)

def bimodal(x, mu1, sigma1, A1, mu2, sigma2, A2):
	return gauss(x, mu1, sigma1, A1) + gauss(x, mu2, sigma2, A2)


def mine():
	pdfs = numpy.random.randn(samples, 2)
	pdfs[:, 0] *= sigmaP + muP
	pdfs[:, 1] *= sigmaC + muC
	# pdfC = numpy.random.randn(samples, ) * sigmaC + muC
	pdfs = numpy.uint16(numpy.clip(pdfs, 0, maxval))

	#for pdf in [pdfP, pdfC]:
	#	pdf = numpy.clip(pdf, 0, maxval)
	#	pdf = numpy.uint16(pdf)

	plt.plot(pdfs)
	plt.show()


def mine_ok():
	data = pylab.concatenate((pylab.normal(muP, sigmaP, samplesP), pylab.normal(muC, sigmaC, samplesC)))
	y, x, _ = pylab.hist(data, 100, alpha=.3, label='data')

	# x = x[1:]
	x = (x[1:] + x[:-1]) / 2 # for len(x)==len(y)  # TODO what?

	expected = (muP, sigmaP, aP, muC, sigmaC, aC)

	params, cov = curve_fit(bimodal, x, y, expected)
	sigma = numpy.sqrt(numpy.diag(cov))
	plt.plot(x, bimodal(x, *params), color='red', lw=3, label='model')
	plt.legend()
	print('params: ', end='')
	print(params)
	print('sigma: ', end='')
	print(sigma)


def other():
	data = pylab.concatenate((pylab.normal(1, .2, 5000), pylab.normal(2, .2, 2500)))
	y, x, _ = pylab.hist(data, 100, alpha=.3, label='data')

	# x = x[1:]
	x = (x[1:] + x[:-1]) / 2 # for len(x)==len(y)  # TODO what?

	expected = (1, .2, 250, 2, .2, 125)
	params, cov = curve_fit(bimodal, x, y, expected)
	sigma = numpy.sqrt(numpy.diag(cov))
	plt.plot(x, bimodal(x, *params), color='red', lw=3, label='model')
	plt.legend()
	print(params, '\n', sigma)


# other()
mine_ok()
plt.show()
