/*
 * http://www.cplusplus.com/forum/general/221172/
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <cmath>

#include "boostNormal.hpp"

double rnd();


/**
 * The function to integrate between 0 and 1
 * @param x
 */
double function (double x)
{
	return (x + 0.3 * x * x - 0.3 * x * x * x);
}

/**
 * Calculate the area behind the curve where you divide area in small squares of sizes i and j
 * @param s
 */
void Monte_Carlo_1 (long & s)
{
	double x = rnd();
	double y = rnd();

	if(y < function(x))
	{
		s++;
	}
}

void Monte_Carlo_2 (long & d);
void Monte_Carlo_3 (double x, long & f);


int main()
{
	double x;

	long max = 5, n = 0;
	long s = 0, d = 0, f = 0;

	const uint ITERS = 15;
	const float BOH = 0.5;

	std::cout << "\n*** Integralas ( I - 0.5 ): Monte Karlo metodas ***\n";
	std::cout << "\nAnalytic Result = 0.0250 \n \n";
	std::cout << "Iter. \t I \t II \t III \n \n";

	for (int j=0; j < ITERS; j++)
	{
		for (int i = 0; i < max; i++)
		{
			Monte_Carlo_1(s);
			Monte_Carlo_2(d);
			Monte_Carlo_3(x, f);
			n++;
		}
		const double MC1_VAL = (double)s / n - BOH;
		printf("%6ld %7.4lf %7.4lf %7.4lf \n", max, MC1_VAL, (double)d / n - BOH, f / n - BOH);
		max *= 2;
	}

	boost_normal();
}


double rnd()
{
	return (double)rand() / (RAND_MAX) ;
}


void Monte_Carlo_2 (long & d)
{
	d += rnd();
}


void Monte_Carlo_3 (double x, long & f)
{
	x = sqrt(rnd());
	f += (x / (1 + 0.3 * x * (1 - x)) / x);
}
