//============================================================================
// Name        : EclipseTbb.cpp
// Author      : Patrizio Bertoni
// Version     :
// Copyright   : Xnext s.r.l. 2020
// Description : Hello World in C++, Ansi-style
// Source      : https://software.intel.com/en-us/node/505530
//============================================================================

#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <string>
#include <cassert>

#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

using namespace tbb;
using t_tp = std::chrono::time_point<std::chrono::high_resolution_clock>;
static const size_t N = 7;

t_tp tic()
{
	return std::chrono::high_resolution_clock::now();
}

double tictoc(t_tp _tic, t_tp _toc)
{
	return (1.e-9) * std::chrono::duration_cast<std::chrono::nanoseconds>(_toc - _tic).count();
}

double toc(t_tp _tic)
{
	return tictoc(_tic, tic());
}

void toclabel(t_tp tic1, std::string lbl)
{
	std::cout << "[" << lbl << "]\t" << std::scientific << toc(tic1) << std::endl;
}

long fibonacci_serial(long n)
{
	return n < 2 ? n : fibonacci_serial(n-1) + fibonacci_serial(n-2);
}



class SubStringFinder
{
	const std::string str;
	size_t * max_array, * pos_array;

public:
	void operator() (const blocked_range<size_t> & r) const
	{
		for (size_t i = r.begin(); i != r.end(); ++i)
		{
			size_t max_size = 0, max_pos = 0;
			for (size_t j = 0; j < str.size(); ++j)
			{
				if (j != i)
				{
					const size_t limit = str.size() - std::max(i, j);
					for (size_t k = 0; k < limit; ++k)
					{
						if (str[i + k] != str[j + k])
							break;
						if (k > max_size)
						{
							max_size = k;
							max_pos = j;
						}
					}
				}
			}
			max_array[i] = max_size;
			pos_array[i] = max_pos;
		}
	}

	SubStringFinder(std::string & s, size_t * m, size_t * p) :
		str(s), max_array(m), pos_array(p)
	{}
};


int main()
{
	std::string str[N] = { std::string("a"), std::string("b") };
	for (size_t i = 2; i < N; ++i)
		str[i] = str[i - 1] + str[i - 2];

	auto & to_scan = str[N - 1];
	const size_t num_elem = to_scan.size();
	assert(num_elem == fibonacci_serial(N));
	std::cout << "sample substring to_scan: " << to_scan << " of " << num_elem << " elements" << std::endl;

	size_t * max = new size_t[num_elem];
	size_t * pos = new size_t[num_elem];

	parallel_for(blocked_range<size_t>(0, num_elem),
		SubStringFinder(to_scan, max, pos));

	std::cout << "idx\tmax\tpos\n";
	for (size_t i = 0; i < num_elem; ++i)
		std::cout << i << "\t" << max[i] << "\t" << pos[i] << std::endl;

	delete[] pos;
	delete[] max;
	return 0;
}
