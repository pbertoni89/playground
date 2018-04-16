#include "ParReduce.hpp"

#include <tbb/task_scheduler_init.h>

#include <vector>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>



class NumberPrinter
{
	/**
	 * perform our local computation. i is the number to compute: display it and returns a string version of it
	 */
	static char * develop(size_t i);

public:
	/**
	 * store the output data of the computation
	 */
	std::vector<char *> m_vcOutput;

	/**
	 * Function that is called to loop through the items assigned to this instance of the class.
	 * TBB uses a blocked_range<size_t> to present the data to class instances.
	 * Here, we just call develop(i) on every item, and save the result in my_vector
	 */
	void operator()(const tbb::blocked_range<size_t> & range);

	/**
	 * copy constructor is called a split constructor,
	 * because it has a useless argument so that Intel TBB differentiates it from a normal copy constructor
	 * 
	 * TBB calls this split constructor when it decides that there is too much data too be handled by a single core.
	 * It then creates a copy of one of the class instances, and distributes the data evenly among them
	 * (this is entirely transparent for the user, and it's one of the many things that makes Intel TBB so cool)
	*/
	NumberPrinter(NumberPrinter & other, tbb::split);

	/**
	 * There are two constructors defined: These just match the requirements of Intel TBB to make the class run
	 */
	NumberPrinter() = default;

	/**
	 * Called by TBB when all the local computations are done on several processes that manage
	 * adjacent data in the original blocked_range. TBB will always call join(const NumberPrinter& y)
	 * in the order in which the data has been distributed.
	 *
	 * For instance, if processes a, b and c are all given a third of the data(in that order),
	 * then a will always join with b and b with c.
	 * This can be easily checked : just print my_vector after the joining has been done and you'll notice
	 * that in there, all the elements are sorted.
	 */
	void join(const NumberPrinter & other);
};



void example_parallel_reduce_number_printer(int n)
{
	NumberPrinter np;

	tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), // range of integers between 0 and n: input data
					np,
					tbb::auto_partitioner()); // how many parts input data should be split, depending on CPUs
}



char * NumberPrinter::develop(size_t i)
{
	// HOW CAN ONE DO A LOG2(0) ?????????????
	i += (i ? 0 : 1);

	const size_t to_malloc = (size_t)((std::log2(i) + 3) * sizeof(char));
	char * foo = (char*) malloc(to_malloc);

	// will mess up buffer between i and std::endl
	//std::cerr << i << std::endl;
	fprintf(stderr, "%zu malloc %zu\n", i, to_malloc);
/*
	char printmsg[100];
	//sprintf(printmsg, "%Iu\t%Iu\t%p\n", i, to_malloc, foo);
	fprintf(stderr, printmsg);
*/
	if (foo == nullptr)
	{
		exit(-1);
	}

	sprintf(foo, "s%zu", i);

	return foo;
}


void NumberPrinter::operator()(const tbb::blocked_range<size_t> & BLOCKED_RANGE)
{
	for (size_t i = BLOCKED_RANGE.begin(); i != BLOCKED_RANGE.end(); ++i)
	{
		m_vcOutput.push_back(develop(i));
	}
}


NumberPrinter::NumberPrinter(NumberPrinter & x, tbb::split)
{}


void NumberPrinter::join(const NumberPrinter& OTHER)
{
	m_vcOutput.reserve(m_vcOutput.size() + OTHER.m_vcOutput.size());

	for (int i = 0; i < OTHER.m_vcOutput.size(); ++i)
	{
		m_vcOutput.push_back(OTHER.m_vcOutput.at(i));
	}

	// my_vect.insert(my_vect.end(), y.my_vect.begin(), y.my_vect.end());
}