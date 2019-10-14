#include "ParReduce.hpp"

#include <tbb/tbb_stddef.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/cache_aligned_allocator.h>

#include <iostream>

/**
 *  specifies details of the reduction, such as how to accumulate subsums and combine them
 */
typedef struct t_reduce_op
{
	float m_sum;

	float * m_x, * m_y;


	t_reduce_op(float * xx, float * yy) :
		m_sum(0), m_x(xx), m_y(yy)
	{}

	/**
	 * This class is Splittable; hence the "copy" constructor
	 * @param other
	 * @param split is a dummy param
	 */
	t_reduce_op(const t_reduce_op & other, tbb::split) :
		m_sum(0), m_x(other.m_x), m_y(other.m_y)
	{}

	/**
	 * do computation.
	 * Different from parallel_for: not const. This is because it must update sum
	 * @param r
	 */
	void operator() (const tbb::blocked_range<int> & r)
	{
		/*
		 * Because the same body might be used to accumulate multiple subranges,
		 * it is critical that operator() not discard earlier accumulations.
		 * An incorrect definition could be float cum_sum = 0;
		 */
		float cum_sum = m_sum;

		for (int i=r.begin(); i!=r.end(); ++i)
		{
			m_x[i] += ( m_y[i+1] + m_y[i-1] )*.5;
			cum_sum += m_y[i] * m_y[i];
		}

		m_sum = cum_sum;
	}

	/**
	 * a method join that must be present for parallel_reduce to work
	 * @param s
	 */
	void join(const t_reduce_op & other)
	{
		m_sum += other.m_sum;
	}
} t_reduce_op;



void example_parallel_reduce_simple(int n)
{
	float * x, * y;
	x = tbb::cache_aligned_allocator<float>().allocate(n + 1);
	y = tbb::cache_aligned_allocator<float>().allocate(n + 1);

	for (int i=0; i<n; i++)
	{
		x[i] = rand() % 50;
		y[i] = rand() % 50 + 50;
	}

	if (n <= 20)
	{
		std::cout << "\nI";
		for (int i=0; i<n; i++)
		{
			std::cout << "\t" << i;
		}
		std::cout << "\nX";
		for (int i=0; i<n; i++)
		{
			std::cout << "\t" << x[i];
		}
		std::cout << "\nY";
		for (int i=0; i<n; i++)
		{
			std::cout << "\t" << y[i];
		}
	}

	t_reduce_op reduceOp(x, y);

	// rules for partitioners and grain sizes for parallel_reduce are the same as for parallel_for
	tbb::parallel_reduce(tbb::blocked_range<int>(0, n, 1024), reduceOp);

	if (n <= 20)
	{
		std::cout << "\nZ";
		for (int i=0; i<n; i++)
		{
			std::cout << "\t" << x[i];
		}
	}
	std::cout << "\nReduce: e = " << reduceOp.m_sum << std::endl;

	tbb::cache_aligned_allocator<float>().deallocate(x, n + 1);
	tbb::cache_aligned_allocator<float>().deallocate(y, n + 1);
}
