#include "NumberPrinter.hpp"

#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void sample_parallel_reduce(int n)
{
	NumberPrinter np;

	tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), // range of integers between 0 and n: input data
					np,
					tbb::auto_partitioner()); // how many parts input data should be split, depending on CPUs
}



char * NumberPrinter::develop(size_t i)
{
	// HOW CAN ONE DO A LOG2(0) ?????????????
	if (i == 0)
	{
		i = 1;
	}

	size_t to_malloc = (size_t)((log2(i) + 3) * sizeof(char));
	char * foo = (char*) malloc(to_malloc);

	// will mess up buffer between i and std::endl
	//std::cerr << i << std::endl;
	fprintf(stderr, "%zu\n", i);
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


void NumberPrinter::join(const NumberPrinter& y)
{
	m_vcOutput.reserve(m_vcOutput.size() + y.m_vcOutput.size());

	for (int i = 0; i < y.m_vcOutput.size(); ++i)
	{
		m_vcOutput.push_back(y.m_vcOutput.at(i));
	}

	// my_vect.insert(my_vect.end(), y.my_vect.begin(), y.my_vect.end());
}