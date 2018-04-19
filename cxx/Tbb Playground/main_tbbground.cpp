#include <tbb/tbb.h>

#include "ParReduce.hpp"
#include "ParFor.hpp"
#include "Fibonacci.hpp"
#include "utils.hpp"

#include <iostream>
#include <chrono>

// --------------------------------------------------------------------------------------------------------

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
#define EXAMPLE_TASKS


// --------------------------------------------------------------------------------------------------------

	
int main()
{
	long S, F;
#if defined(EXAMPLE_PARALLEL_REDUCE) || defined(EXAMPLE_PARALLEL_FOR)
	std::cout << "Insert Parallel Size: ";
	std::cin >> S;
#endif
#if defined(EXAMPLE_THREAD_POOL) || defined(EXAMPLE_TASKS)
	std::cout << "Insert Fibonacci Arg: ";
	std::cin >> F;
#endif


	/*
	 * initialize once the Intel TBB library
	 * Intel® Threading Building Blocks (Intel® TBB) 2.2 and later automatically
	 * initializes the task scheduler.
	 * You can use class task_scheduler_init to explicitly initialize the task
	 * scheduler, which can be useful for doing any of the following:
	 *  - Control when the task scheduler is constructed and destroyed
	 *  - Specify the number of threads used by the task scheduler
	 *  - Specify the stack size for worker threads
	 */
	tbb::task_scheduler_init init;


#ifdef EXAMPLE_PARALLEL_REDUCE
	std::cout << "  ~  ~   ~  ~   ~ PARALLEL REDUCE ~   ~  ~   ~  ~   ~  ~   ~  ~ " << std::endl;
	// bah example_parallel_reduce_number_printer(N);
	example_parallel_reduce_simple(S);
	example_parallel_reduce_min_value(S);
#endif


#ifdef EXAMPLE_PARALLEL_FOR
	std::cout << "  ~  ~   ~  ~   ~ PARALLEL FOR ~   ~  ~   ~  ~   ~  ~   ~  ~ " << std::endl;
	example_parallel_for_explore();
	size_t GRAIN;
	std::cout << "Insert Grain Size: ";
	std::cin >> GRAIN;
	example_parallel_for(S, GRAIN);
#endif



#ifdef EXAMPLE_THREAD_POOL
	std::cout << "  ~  ~   ~  ~   ~ THREAD POOL ~   ~  ~   ~  ~   ~  ~   ~  ~ " << std::endl;
	FibThreadPool tp;
	using e_em = FibThreadPool::e_exec_mode;
	tp.test(e_em::RUN_ALL_THEN_WAIT, F); // N=50: 28 s
	// tp.test(e_em::RUN_WAIT, F);       // N=50: 68 s
	tp.test(e_em::THREADS, F);           // N=50: 28 s
#endif


#ifdef EXAMPLE_TASKS
	std::cout << "  ~  ~   ~  ~   ~ TASKS  ~   ~  ~   ~  ~   ~  ~   ~  ~ " << std::endl;
													const auto t0 = tic();
	const long FN = fibonacci_serial(F);			// N=50: 43 s
													toclabel(t0, "SERIAL: " + std::to_string(FN));
													const auto t1 = tic();
	const long F2 = fibonacci_task(F);				// N=50: 500 s !!!!
													toclabel(t1, "TASKS: " + std::to_string(F2));
													const auto t2 = tic();
	const long F3 = fibonacci_task_tree(F);			// N=50: LASCIA PERDERE
													toclabel(t2, "TASKTR: " + std::to_string(F3));
	if (FN != F2 or F2 != F3)
	{
		throw std::runtime_error("Fibonacci was wrong");
	}
#endif

	return 0;
}
