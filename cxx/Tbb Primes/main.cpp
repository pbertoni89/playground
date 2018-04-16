/*
    ==============================================================

    SAMPLE SOURCE CODE - SUBJECT TO THE TERMS OF SAMPLE CODE LICENSE AGREEMENT,
    http://software.intel.com/en-us/articles/intel-sample-source-code-license-agreement/

    Copyright 2016 Intel Corporation

    THIS FILE IS PROVIDED "AS IS" WITH NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE, NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS.

    =============================================================
*/

#include "primes.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <utility>
#include <iostream>
#include <sstream>
#include "tbb/tick_count.h"

#include "../tbb-utility.h"


typedef struct t_run_options
{
	//! NumberType of threads to use.
	utility::thread_number_range threads;

	//whether to suppress additional output
	bool silentFlag;

	//
	NumberType n;

	//! Grain size parameter
	NumberType grainSize;

	// number of time to repeat calculation
	NumberType repeatNumber;

	t_run_options(utility::thread_number_range threads, NumberType grainSize, NumberType n, bool silentFlag, NumberType repeatNumber)
		: threads(threads), grainSize(grainSize), n(n), silentFlag(silentFlag), repeatNumber(repeatNumber)
	{}

	friend std::ostream & operator << (std::ostream & out, const t_run_options & OP)
	{
		out << "RUN_OPTIONS\n\tthreads: " << OP.threads << "\n\tn: " << OP.n << "\n\tgrainSize: " << OP.grainSize << "\n\trepeatNumber: " << OP.repeatNumber << std::endl;
	}
} t_run_options;


int do_get_default_num_threads()
{
	int threads;
	#if __TBB_MIC_OFFLOAD
	#pragma offload target(mic) out(threads)
	#endif // __TBB_MIC_OFFLOAD
	threads = tbb::task_scheduler_init::default_num_threads();
	return threads;
}


int get_default_num_threads()
{
	static int threads = do_get_default_num_threads();
	return threads;
}

/**
 * Parse the command line
 */
static t_run_options ParseCommandLine(int argc, const char* argv[])
{
	utility::thread_number_range threads(get_default_num_threads, 0, get_default_num_threads());
	NumberType grainSize = 1000;
	bool silent = false;
	NumberType number = 100000000;
	NumberType repeatNumber = 1;

	utility::parse_cli_arguments(argc,argv,
		utility::cli_argument_pack()
			//"-h" option for displaying help is present implicitly
			.positional_arg(threads, "n-of-threads", utility::thread_number_range_desc)
			.positional_arg(number, "number", "upper bound of range to search primes in, must be a positive integer")
			.positional_arg(grainSize, "grain-size", "must be a positive integer")
			.positional_arg(repeatNumber, "n-of-repeats", "repeat the calculation this number of times, must be a positive integer")
			.arg(silent, "silent", "no output except elapsed time")
	);

	const t_run_options options(threads, grainSize, number, silent, repeatNumber);
	return options;
}


int main(int argc, const char* argv[])
{
											const tbb::tick_count mainBeginMark = tbb::tick_count::now();
	const t_run_options options = ParseCommandLine(argc,argv);
											std::cout << options;

	// Try different numbers of threads
	for (int p=options.threads.first; p<=options.threads.last; p=options.threads.step(p))
	{
		for (NumberType i=0; i<options.repeatNumber; ++i)
		{
											const tbb::tick_count iterationBeginMark = tbb::tick_count::now();
			NumberType count = 0;

			if (p==0)
			{
																				#if __TBB_MIC_OFFLOAD
																				#pragma offload target(mic) in(N) out(count)
																				#endif // __TBB_MIC_OFFLOAD
				count = primes_count_serial(options.n);
			}
			else
			{
																				#if __TBB_MIC_OFFLOAD
																				#pragma offload target(mic) in(N, p, grainSize) out(count)
																				#endif // __TBB_MIC_OFFLOAD
				count = primes_count_parallel(options.n, p, options.grainSize);
			}

											const tbb::tick_count iterationEndMark = tbb::tick_count::now();
											if (!options.silentFlag)
											{
												std::cout << "#primes from [2.." << options.n << "] = " << count
															<< " (" << (iterationEndMark-iterationBeginMark).seconds() << " sec with ";
												if (p != 0)
													std::cout << p << "-way parallelism";
												else
													std::cout << "serial code";
												std::cout << ")\n" ;
											}
		}
	}
											utility::report_elapsed_time((tbb::tick_count::now() - mainBeginMark).seconds());
	return 0;
}
