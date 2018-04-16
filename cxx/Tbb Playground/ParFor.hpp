#pragma once

#include "tbb/tbb.h"

#include <iostream>
#include <sstream>


void example_parallel_for_explore();


typedef struct t_perf
{
	double time_par, time_ser;
} t_perf;


/**
 * The blocked_range constructed here represents the entire iteration space from 0 to n-1,
 * which parallel_for divides into subspaces for each processor.
 * The general form of the constructor is blocked_range<T>(begin,end,grainsize).
 * The T specifies the value type.
 * The arguments begin and end specify the iteration space STL-style as a half-open interval [begin,end).
 * The argument grainsize is explained in the Controlling Chunking section.
 * The example uses the default grainsize of 1 because by default parallel_for applies a heuristic
 * that works well with the default grainsize.
 *
 * @param DIM the vector size
 *
 * @param The grainsize sets a minimum threshold for parallelization.
 * The parallel_for in the example invokes ApplyFoo::operator() on chunks,
 * possibly of different sizes. Let chunksize be the number of iterations in a chunk.
 * Using simple_partitioner guarantees that ⌈G/2⌉ ≤ chunksize ≤ G.
 */
t_perf example_parallel_for(size_t DIM, size_t GRAINSIZE);


/**
 * A logarithmic unary operation
 *
 * @template T is the arithmetic type
 * @template Op is the unary operation from T to T
 */
template <typename T, T (* Op)(T)>
class ParOp
{
public:
	ParOp()
	{}

	T operator() (T arg) const
	{
		return Op(arg);
	}
};

/*
 * The iteration space here is of type size_t, and goes from 0 to n-1.
 * The template function tbb::parallel_for breaks this iteration space into chunks,
 * and runs each chunk on a separate thread.
 * The first step in parallelizing this loop is to convert the loop body into a form that operates on a chunk.
 * The form is an STL-style function object, called the body object, in which operator() processes a chunk.
 * The following code declares the body object.
 */
class ParForLogTask
{
	/**
	 * Reference to read-only input
	 */
	const std::vector<float> & m_in;

	/**
	 * Substitute with template TemplateOp
	 */
	const ParOp<float, std::log2> _OP;
	
public:

	/**
	 * Reference to write output. Using local object is dangerous (at a certain
	 * point after usage, they are all equal to 0). See operator() doc
	 */
	std::vector<float> & m_out;

	/**
	 * TBB operator override which contains the ACTUAL PARALLELIZED CYCLE.
	 *
	 * Because the body object might be copied, its operator() SHOULD NOT modify the body.
	 * Otherwise the modification might or might not become visible to the thread that invoked parallel_for,
	 * depending upon whether operator() is acting on the original or a copy.
	 * As a reminder of this nuance, parallel_for requires that the body object's operator() be declared const.
	 *
	 * @param A blocked_range<T> describes a one-dimensional iteration space over type T.
	 * Class parallel_for works with other kinds of iteration spaces too.
	 * The library provides blocked_range2d for two-dimensional spaces.
	 * You can define your own spaces as explained in Advanced Topic: Other Kinds of Iteration Spaces.
	 *
	 * The example operator() loads my_a into a local variable a.
	 * Though not necessary, there are two reasons for doing this in the example:
	 * 	- Style. It makes the loop body look more like the original.
	 * 	- Performance. Sometimes putting frequently accessed values into local variables helps the compiler
	 * 	optimize the loop better, because local variables are often easier for the compiler to track.
	 */
	void operator()(const tbb::blocked_range<size_t> & range) const;

	/**
	 * Usually, the constructor for the body object will initialize instance attributes,
	 * though parallel_for does not care how the body object is created.
	 *
	 * Template function parallel_for requires that the body object have a copy constructor,
	 * which is invoked to create a separate copy (or copies) for each worker thread.
	 * It also invokes the destructor to destroy these copies.
	 * In most cases, the implicitly generated copy constructor and destructor work correctly.
	 * If they do not, it is almost always the case (as usual in C++) that you must define both to be consistent.
	 */
	ParForLogTask(const std::vector<float> & v_in, std::vector<float> & v_out);
};
