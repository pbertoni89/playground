#include <tbb/tbb.h>

#include "ParFor.hpp"
#include "Tasks.hpp"
#include "NumberPrinter.hpp"
#include "ParReduce.hpp"
#include "ThreadPool.hpp"
#include "utils.hpp"

#include <iostream>
#include <chrono>

// --------------------------------------------------------------------------------------------------------

//#define EXAMPLE_SIMPLE_THREAD

/**
 * http://www.mupuf.org/blog/2011/11/13/parallel_programming_hello_world_with_intel_tbb
 */
#define EXAMPLE_PARALLEL_REDUCE

/**
 * https://software.intel.com/en-us/node/506057
 */
// #define EXAMPLE_PARALLEL_FOR

/**
 * 
 */
// #define EXAMPLE_THREAD_POOL

/**
 * https://www.threadingbuildingblocks.org/tutorial-intel-tbb-task-based-programming/
 */
// #define EXAMPLE_TASKS


// --------------------------------------------------------------------------------------------------------

	
int main()
{
	long N = 0;
	std::cout << "Insert Parallel Size (Fibonacci Arg, Vector Length): ";
	std::cin >> N;



#ifdef EXAMPLE_THREAD_POOL
	ThreadPoolExample tp;
	tp.test(e_exec_mode::RUN_ALL_THEN_WAIT, N);	// ABOUT 2 seconds
	tp.test(e_exec_mode::RUN_WAIT, N);			// ABOUT 5 seconds
	tp.test(e_exec_mode::THREADS, N);			// ABOUT 2 seconds
#else
	/*
	 * initialize once the Intel TBB library
	 * Intel® Threading Building Blocks (Intel® TBB) 2.2 and later automatically initializes the task scheduler.
	 * You can use class task_scheduler_init to explicitly initialize the task scheduler,
	 * which can be useful for doing any of the following:
	 *  - Control when the task scheduler is constructed and destroyed
	 *  - Specify the number of threads used by the task scheduler
	 *  - Specify the stack size for worker threads
	 */
	tbb::task_scheduler_init init;
#endif

	
#ifdef EXAMPLE_SIMPLE_THREAD
	simple_threads();
#endif


#ifdef EXAMPLE_PARALLEL_REDUCE
	example_parallel_reduce_number_printer(N);
	example_parallel_reduce_simple(N);
#endif


#ifdef EXAMPLE_PARALLEL_FOR
	std::cout << "  ~  ~   ~  ~   ~ PARALLEL FOR ~   ~  ~   ~  ~   ~  ~   ~  ~ " << std::endl;
	example_parallel_for_explore();
	size_t GRAIN;
	std::cout << "Insert Grain Size: ";
	std::cin >> GRAIN;
	example_parallel_for(N, GRAIN);
#endif


#ifdef EXAMPLE_TASKS
	std::cout << "  ~  ~   ~  ~   ~ TASKS  ~   ~  ~   ~  ~   ~  ~   ~  ~ " << std::endl;
													const auto t0 = tic();
	const long FN = fibonacci_serial(N);
													toclabel(t0, "SERIAL");
													const auto t1 = tic();
	const long F2 = fibonacci_task(N);
													toclabel(t1, "TASKS ");
													const auto t2 = tic();
	const long F3 = fibonacci_task_tree(N);
													toclabel(t2, "TASKTR");
	if (FN != F2 || F2 != F3)
	{
		throw std::runtime_error("Fibonacci was wrong");
	}
#endif

	return 0;
}
