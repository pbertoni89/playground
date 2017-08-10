"""
Created on Wed Aug  9 07:38:52 2017

## aaaazz #!/usr/bin/env python2
## aaaazz # -*- coding: utf-8 -*-

https://stackoverflow.com/questions/7903977/implementing-a-kolmogorov-smirnov-test-in-python-scipy/7904652#7904652

@author: patrizio
"""

import numpy as np
import scipy.stats as stats
import sys



def foo_kstest():
	print("executing foo_kstest")
	return 645



def run_kstest(vData, alpha = 0.05):
	"""
		VC is standardized here
	"""

	# Won't work if input is a list
	vData = np.array(vData)
	mu = vData.mean()
	sigma = vData.std()

	# Standardize
	normData = vData - mu
	if sigma:
		normData /= sigma

	# SciPy call
	ksResult = stats.kstest(normData, 'norm')

	testPassed = False
	if ksResult.pvalue >= alpha:
		# We cannot reject the null hypotesis !
		testPassed = True

	print("VC({:.5f}, {:.5f}) -> D {:.5f}, pvalue {:.5f} -> Passed: {}".format(mu, sigma, ksResult.statistic, ksResult.pvalue, testPassed))
	# OK print("In facts, {:.5f}, {:.5f}".format(normData.mean(), normData.std()))
	return {"KstestResult": ksResult, "passed": testPassed}



def ksverdict(vData):
	"""
		Simplest wrapper
	"""

	retval = run_kstest(vData, alpha = 0.05)

	if retval["passed"] is True:
		# print("Test passed")
		return 1
	else:
		# print("Test NOT passed")
		return 0



def example_n(mu, sigma, nc = 10000):
	"""
		Run a test with a N(mu, sigma)
	"""
	vData = np.random.normal(mu, sigma, nc)
	return run_kstest(vData)



def example_u(low, high, nc = 10000):
	"""
	Run a test with a U(low, high)
	"""
	vData = np.random.uniform(low, high, nc)
	return run_kstest(vData)



def lots_of_std_tests(mu = 0.3, sigma = 5, num_tests = 10**5, nc = 10000, alpha = 0.05):
	"""
		KS Proof Demo
	"""

	test_percentile = num_tests/10
	num_rejects = 0

	"""
	https://stackoverflow.com/questions/17192158/nameerror-global-name-xrange-is-not-defined-in-python-3
	python2 xrange become python3 range
	"""
	if sys.version_info.major == 3:
		num_tests_range = xrange(num_tests)
	else:
		num_tests_range = range(num_tests)

	for i in num_tests_range:
		vData = np.random.normal(mu, sigma, nc)
		ksDict = run_kstest(vData, alpha)
		# print(ksDict)
		if ksDict["passed"] == False:
			num_rejects += 1

		if num_tests > 10**4 and i % test_percentile == 0:
			print('{}0 %'.format(i / test_percentile))

	ratio = float(num_rejects) / num_tests
	print('{}/{} = {:.2f} % rejects at rejection level alpha {}'.format(num_rejects, num_tests, ratio, alpha))



#  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


print("Welcome to Xnext KS Test. Running Python {}.{}".format(sys.version_info.major, sys.version_info.minor))

# lots_of_std_tests(num_tests = 10000)

# example_n(3, 4)

# example_u(0, 1)
