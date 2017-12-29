#pragma once

//#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>

#include <list>

#include <cmath>

#include <tbb/task.h>
#include <tbb/task_group.h>
#include "tbb/task_scheduler_init.h"
#include <tbb/compat/thread>

#define N_THREADS_IN_POOL 4
#define EXECUTIONS 40

#define THREAD_AND_WAIT

/** Main Reference for TASK_GROUP
	https://www.threadingbuildingblocks.org/docs/help/index.htm#reference/task_groups/task_group_cls.html		*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class FibonacciTask
{
private:
	long _iFibStart;
	long _iResult;
	size_t _iSpaces;
	std::string _sTabs;

public:
	FibonacciTask(long fibStart);
	long operator()() const;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum ExecutionMode
{
	RUN_ALL_THEN_WAIT,
	RUN_WAIT,
	THREADS
};

/**
	Creating a large number of tasks for a single task_group is not scalable,
	because task creation becomes a serial bottleneck.
	If creating more than a small number of concurrent tasks,
	consider using parallel_for or parallel_invoke instead, or structure the spawning as a recursive tree		*/

class ThreadPoolExample
{
private:
	tbb::task_scheduler_init init;
	tbb::task_group group;

public:
	ThreadPoolExample();

	/** @param mode.				* * * 'RUN_ALL_THEN_WAIT' SEEMS FASTER * * *
	
		RUN_WAIT - Equivalent to {run(f); wait();}, but guarantees that f runs on the current thread.
				Template method run_and_wait is intended to be more efficient than separate calls to run and wait

		RUN_ALL_THEN_WAIT - Wait for all tasks in the group to complete or be cancelled
				Returns: True if this task group is cancelling its tasks
			
		THREADS - Use C++11 threads TBB implementation															*/
	void run(ExecutionMode mode);
};