#include "ThreadPoolExample.hpp"

long fibonacci(long a)
{
	if (a < 2)
		return 1;
	else
		return fibonacci(a - 1) + fibonacci(a - 2);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FibonacciTask::FibonacciTask(long fibStart)
{
	static int __iTotalTasksRun = 0;

	_iFibStart = fibStart;

	_iSpaces = __iTotalTasksRun % N_THREADS_IN_POOL;

	for (int i = 0; i < _iSpaces; i++)
		_sTabs += "\t\t";

	__iTotalTasksRun ++;
}

long FibonacciTask::operator()() const
{
	std::stringstream ss1;
	ss1 << _sTabs << "S " << std::setw(3) << _iFibStart << std::endl;
	std::cout << ss1.str();

	long localRes = fibonacci(_iFibStart);

	std::stringstream ss2;
	ss2 << _sTabs << "E " << std::setw(3) << _iFibStart << /*"= " << localRes <<*/ std::endl;
	std::cout << ss2.str();

	//_iResult = localRes;				// CANNOT: CONST OPERATOR 
	return localRes;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ThreadPoolExample::ThreadPoolExample() :
	init(N_THREADS_IN_POOL)
{
}

void ThreadPoolExample::run(ExecutionMode mode)
{
	auto t0 = std::chrono::high_resolution_clock::now();

	std::vector<std::thread> vt;

	for (long i = 0; i < EXECUTIONS; ++i)
	{
		switch (mode)
		{
		case ExecutionMode::RUN_WAIT:
			group.run_and_wait(FibonacciTask(i));
			break;
		case ExecutionMode::RUN_ALL_THEN_WAIT:
			group.run(FibonacciTask(i));
			break;
		case ExecutionMode::THREADS:
			vt.push_back(std::thread(fibonacci, i));
			break;
		}

		std::stringstream ss;
		ss << "Spawned iteration " << i << std::endl;
		std::cout << ss.str();
	}

	std::cout << "Spawned all iterations" << std::endl;

	switch (mode)
	{
	case ExecutionMode::RUN_WAIT:
		break;
	case ExecutionMode::RUN_ALL_THEN_WAIT:
		group.wait();
		break;
	case ExecutionMode::THREADS:
		for (long i = 0; i < EXECUTIONS; ++i)
		{
			vt.at(i).join();
		}
		break;
	}

	auto t1 = std::chrono::high_resolution_clock::now();

	auto dt = (1.e-9) * std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
	std::cout << "Total elapsed time is " << dt << " seconds for " << mode << std::endl;
}
