#pragma once

#include "tbb/tbb.h"

#include <iostream>
#include <sstream>



/**
 * Once you have the loop body written as a body object,
 * invoke the template function parallel_for, as follows
 * 
 * The blocked_range constructed here represents the entire iteration space from 0 to n-1,
 * which parallel_for divides into subspaces for each processor.
 * The general form of the constructor is blocked_range<T>(begin,end,grainsize).
 * The T specifies the value type.
 * The arguments begin and end specify the iteration space STL-style as a half-open interval [begin,end).
 * The argument grainsize is explained in the Controlling Chunking section.
 * The example uses the default grainsize of 1 because by default parallel_for applies a heuristic
 * that works well with the default grainsize.
 */
void sample_parallel_for(float a[], size_t n);

/*
 * The iteration space here is of type size_t, and goes from 0 to n-1.
 * The template function tbb::parallel_for breaks this iteration space into chunks,
 * and runs each chunk on a separate thread.
 * The first step in parallelizing this loop is to convert the loop body into a form that operates on a chunk.
 * The form is an STL-style function object, called the body object, in which operator() processes a chunk.
 * The following code declares the body object.
 */
class FooTask
{
	struct foo_struct
	{
		int a;
	};

	/**
	 * An instance needs member fields that remember all the local variables
	 * that were defined outside the original loop but used inside it.
	 */

	/**
	 * Read-only data structure
	 */
	float * const _pInput;

	/**
	 * Write-only
	 */
	float * _pOutput;

	/**
	 * This is a real object: it cannot be modified
	 */
	int _iCounterNotModifyable;

	/**
	 * This is just a pointer to something: it can !
	 */
	int * _piCounterModifyable;

	/**
	 * This is just a pointer to something: it can !
	 */
	foo_struct * _pFooStruct;
	
public:

	/**
	 * Actual parallel method
	 */
	void read(float a, int pos) const;

	void write(float a, int pos);

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
	void operator()(const tbb::blocked_range<size_t> & r) const;

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
	FooTask(float a[]);

	~FooTask();

	/**
	 * Experimental workaround to reduce dtor parallelism
	 */
	void meta_destructor();
};
