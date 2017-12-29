#include <iostream>
#include <tbb/tbb.h>

// --------------------------------------------------------------------------------------------------------

//#define EXAMPLE_SIMPLE_THREAD
/**  http://www.mupuf.org/blog/2011/11/13/parallel_programming_hello_world_with_intel_tbb			*/
//#define EXAMPLE_PARALLEL_REDUCE
/**  https://software.intel.com/en-us/node/506057													*/
//#define EXAMPLE_PARALLEL_FOR
/** ? ? ?																							*/
//#define EXAMPLE_THREAD_POOL
/** https://www.threadingbuildingblocks.org/tutorial-intel-tbb-task-based-programming/				*/
#define EXAMPLE_TASK_BASED

// --------------------------------------------------------------------------------------------------------

#ifdef EXAMPLE_SIMPLE_THREAD
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

#ifdef EXAMPLE_PARALLEL_FOR
#include "PseudoMadaProducer.hpp"
#endif

#ifdef EXAMPLE_PARALLEL_REDUCE
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>
#include "NumberPrinter.hpp"

void printNumbers(int n)
{
	NumberPrinter p(n);

	parallel_reduce(blocked_range<size_t>(0, n), // range of integers between 0 and n: input data
					p,
					auto_partitioner()); // how many parts input data should be split, depending on CPUs
}
#endif

#ifdef EXAMPLE_THREAD_POOL
#include "ThreadPoolExample.hpp"
#endif

#ifdef EXAMPLE_TASK_BASED
#include <tbb/task_group.h>

int tbb_fibonacci(int n)
{
	if (n<2)
	{
		return n;
	}
	else
	{
		int x, y;
		tbb::task_group g;
		g.run([&] {x = tbb_fibonacci(n - 1); });	// spawn a task
		g.run([&] {y = tbb_fibonacci(n - 2); });	// spawn another task
		g.wait();									// wait for both tasks to complete
		return x + y;
	}
}
#endif

// --------------------------------------------------------------------------------------------------------

int main()
{
#ifdef EXAMPLE_SIMPLE_THREAD
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

#ifdef EXAMPLE_PARALLEL_REDUCE
	/** initialize the Intel TBB library	*/
	task_scheduler_init init;
	printNumbers(1000);
	//system("PAUSE");
#endif

#ifdef EXAMPLE_PARALLEL_FOR
	float a[4] = { float(1.2), float(3.1), float(4.2), float(0.9) };
	parallelProduction(a, 4);
#endif

#ifdef EXAMPLE_THREAD_POOL
	ThreadPoolExample tp;
	tp.run(ExecutionMode::RUN_ALL_THEN_WAIT);	// ABOUT 2 seconds
	tp.run(ExecutionMode::RUN_WAIT);			// ABOUT 5 seconds
	tp.run(ExecutionMode::THREADS);				// ABOUT 2 seconds
#endif

#ifdef EXAMPLE_TASK_BASED
	int n = 30;
	std::cout << "Fib(" << n << ") = " << tbb_fibonacci(n) << std::endl;
#endif

	return 0;
}
