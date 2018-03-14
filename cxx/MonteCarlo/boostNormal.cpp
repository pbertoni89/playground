#include "boostNormal.hpp"

#include <iostream>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>


/**
 * Integrates the standard normal distribution between xlow and xhigh
 */
double mcinteg(double xlow, double xhigh, size_t n_iters)
{
	boost::mt19937 igen;

	typedef boost::variate_generator<boost::mt19937, boost::normal_distribution<double> > t_gauss_gen;

	t_gauss_gen gen(igen, boost::normal_distribution<double>());

	size_t n_good = 0;

	for (size_t i=0; i<n_iters; ++i)
	{
		const double x = gen();

		if (x>= xlow and x<xhigh)
		{
			n_good ++;
		}
	}
	return (double)n_good/n_iters;
}


void boost_normal()
{
	const double X_LOW = -1.0, X_HIGH = 1.0;

	const double RES_EXPECTED = 0.5 * (erf(X_HIGH / sqrt(2.0)) - erf(X_LOW / sqrt(2.0))),
		RES_MC = mcinteg(X_LOW, X_HIGH, 1000000);

	std::cout << "MC Result: " << RES_MC << std::endl << "Expected: " << RES_MC << std::endl;
}