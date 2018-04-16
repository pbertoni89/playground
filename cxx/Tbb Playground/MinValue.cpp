#include "ParReduce.hpp"

#include <float.h>


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
	MinIndexFoo mif(a);
	tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), mif);

	return mif.index_of_min;
}
