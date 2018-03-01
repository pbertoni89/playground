#pragma once

/*
 * Main Reference for TASK_GROUP
 * https://www.threadingbuildingblocks.org/docs/help/index.htm#reference/task_groups/task_group_cls.html
 */

#include <tbb/task.h>
#include <tbb/task_group.h>
#include "tbb/task_scheduler_init.h"
#include <tbb/compat/thread>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
#include <list>
#include <cmath>


enum class e_exec_mode
{
	RUN_ALL_THEN_WAIT,
	RUN_WAIT,
	THREADS
};


/**
 * Creating a large number of tasks for a single task_group is not scalable,
 * because task creation becomes a serial bottleneck.
 * If creating more than a small number of concurrent tasks,
 * consider using parallel_for or parallel_invoke instead, or structure the spawning as a recursive tree
 */
class ThreadPoolExample
{
	tbb::task_scheduler_init m_scheduler;

	tbb::task_group m_group;

public:

	/**
	 * Should not (only for printing purposes)
	 */
	static const size_t N_THREADS_IN_POOL = 4;

	/**
	 * Initialize the m_scheduler with N_THREADS_IN_POOL
	 */
	ThreadPoolExample();

	/**
	 * @param mode
	 * * * 'RUN_ALL_THEN_WAIT' SEEMS FASTER <ofc> * * *
	 *
	 * - RUN_WAIT - Equivalent to {run(f); wait();}, but guarantees that f runs on the current thread.
	 *	Template method run_and_wait is intended to be more efficient than separate calls to run and wait
	 *
	 * - RUN_ALL_THEN_WAIT - Wait for all tasks in the group to complete or be cancelled
	 *	Returns: True if this task group is cancelling its tasks
	 *
	 * - THREADS - Use C++11 threads TBB implementation
	 *
	 * @param N - fibonacci argument
	 */
	void test(e_exec_mode mode, long N);
};