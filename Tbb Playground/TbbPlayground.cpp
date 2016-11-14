#include <iostream>
#include <tbb/tbb.h>

//#define THREAD_EXAMPLE
/**  http://www.mupuf.org/blog/2011/11/13/parallel_programming_hello_world_with_intel_tbb			*/
//#define PARALLEL_REDUCE_EXAMPLE
/**  https://software.intel.com/en-us/node/506057													*/
//#define PARALLEL_FOR_EXAMPLE
#define THREAD_POOL_EXAMPLE

#ifdef THREAD_EXAMPLE
#include <tbb/compat/thread>	// introduces std::thread
bool MyThread(int something)
{
	std::cout << "START: something is " << something << std::endl;

	for (int i = 0; i < 20000000; i++)
	{
		something++;
	}

	Sleep(1);
	std::cout << "END: something is " << something << std::endl;

	return true;
}
#endif

#ifdef PARALLEL_FOR_EXAMPLE
#include "PseudoMadaProducer.hpp"
#endif

#ifdef PARALLEL_REDUCE_EXAMPLE
#include "tbb/parallel_reduce.h"
#include "tbb/task_scheduler_init.h"
#include "NumberPrinter.hpp"

using namespace tbb;

void printNumbers(int n)
{
	NumberPrinter p(n);

	parallel_reduce(blocked_range<size_t>(0, n), // range of integers between 0 and n: input data
					p,
					auto_partitioner()); // how many parts input data should be split, depending on CPUs
}
#endif

#ifdef THREAD_POOL_EXAMPLE
#include "ThreadPoolExample.hpp"
#endif

int main()
{
#ifdef THREAD_EXAMPLE
	tbb::tbb_thread pMyThread1(MyThread, 1);
	std::thread araberara1(MyThread, 11);
	tbb::tbb_thread pMyThread2(MyThread, 2);
	std::thread araberara2(MyThread, 12);
	tbb::tbb_thread pMyThread3(MyThread, 3);
	std::thread araberara3(MyThread, 13);
	tbb::tbb_thread pMyThread4(MyThread, 4);
	std::thread araberara4(MyThread, 14);

	pMyThread1.join();
	pMyThread2.join();
	pMyThread3.join();
	pMyThread4.join();

	araberara1.join();
	araberara2.join();
	araberara3.join();
	araberara4.join();
#endif

#ifdef PARALLEL_REDUCE_EXAMPLE
	/** initialize the Intel TBB library	*/
	task_scheduler_init init;
	printNumbers(1000);
	//system("PAUSE");
#endif

#ifdef PARALLEL_FOR_EXAMPLE
	float a[4] = { float(1.2), float(3.1), float(4.2), float(0.9) };
	parallelProduction(a, 4);
#endif

#ifdef THREAD_POOL_EXAMPLE
	ThreadPoolExample tp;
	tp.run(ExecutionMode::RUN_ALL_THEN_WAIT);	// ABOUT 2 seconds
	tp.run(ExecutionMode::RUN_WAIT);			// ABOUT 5 seconds
	tp.run(ExecutionMode::THREADS);				// ABOUT 2 seconds
#endif

	return 0;
}
