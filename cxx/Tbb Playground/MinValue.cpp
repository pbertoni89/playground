#include "ParReduce.hpp"

#include <tbb/tick_count.h>

#include <float.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>


float Foo(float f)
{
	return f;
}


class MinIndexFoo
{
	const float * const my_a;

public:

	float value_of_min;
	long index_of_min;

	void operator()( const tbb::blocked_range<size_t>& r )
	{
		const float *a = my_a;
		for (size_t i=r.begin(); i!=r.end(); ++i )
		{
		   float value = Foo(a[i]);    
		   if (value<value_of_min)
		   {
			   value_of_min = value;
			   index_of_min = i;
		   }
		}
	}


	MinIndexFoo(MinIndexFoo& x, tbb::split) : 
		my_a(x.my_a), 
		value_of_min(FLT_MAX),
		index_of_min(-1) 
	{}


	void join(const MinIndexFoo & y)
	{
		if (y.value_of_min<value_of_min)
		{
			value_of_min = y.value_of_min;
			index_of_min = y.index_of_min;
		}
	}

	MinIndexFoo(const float a[]) :
		my_a(a), 
		value_of_min(FLT_MAX),
		index_of_min(-1)
	{}
};




void example_parallel_reduce_min_value(int n)
{
												int dummy1, dummy2;
												std::cout << "Ready to allocate " << ((n*sizeof(float))/(1024*1024)) << " MiB... Enter dummy1" << std::endl;
												std::cin >> dummy1;
	std::vector<float> v(n);
	std::random_device rnd_device;
	std::mt19937 mersenne_engine(rnd_device());

	std::uniform_real_distribution<float> dist(-1.2, 4.6);

	auto gen = std::bind(dist, mersenne_engine);
	std::generate(std::begin(v), std::end(v), gen);

												std::cout << "Ready to start... Enter dummy2" << std::endl;
												std::cin >> dummy2;
												std::cout << "Entered " << dummy1 << "," << dummy2 << std::endl;
												const auto tcSerStart = tbb::tick_count::now();
	auto itMin = v.cbegin();
	for (auto it=itMin; it!=v.cend(); it++)
	{
		if (*it < *itMin)
		{
			itMin = it;
		}
	}
												const auto tcSerEnd = tbb::tick_count::now();
												std::cout << "min index [ser] " << (itMin - v.cbegin())
													<< " in " << (tcSerEnd-tcSerStart).seconds() << std::endl;
	MinIndexFoo mif(v.data());
												const auto tcParStart = tbb::tick_count::now();

	tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), mif);

												const auto tcParEnd = tbb::tick_count::now();
												std::cout << "min index [par] " << mif.index_of_min
													<< " in " << (tcParEnd-tcParStart).seconds() << std::endl;
}
