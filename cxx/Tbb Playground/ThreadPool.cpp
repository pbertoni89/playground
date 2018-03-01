#include "ThreadPool.hpp"

#include "utils.hpp"

// #define DEBUG_TP

/**
 * We often seamlessy call "a Task" <-> "a Functor"
 */
class FibFunctor
{
	long m_iFibStart;
	long m_iResult;
	size_t m_szSpaces;
	std::string m_sTabs;

public:

	FibFunctor(long fibStart)
	{
		static int __iTotalTasksRun = 0;

		m_iFibStart = fibStart;
#ifdef DEBUG_TP
		m_szSpaces = __iTotalTasksRun % ThreadPoolExample::N_THREADS_IN_POOL;

		for (int i = 0; i < m_szSpaces; i++)
		{
			m_sTabs += "\t\t";
		}
#endif
		__iTotalTasksRun ++;
	}

	long operator()() const
	{
#ifdef DEBUG_TP
							std::stringstream ss1;
							ss1 << m_sTabs << "S " << std::setw(3) << m_iFibStart << std::endl;
							std::cout << ss1.str();
#endif
		long localRes = fibonacci_serial(m_iFibStart);
#ifdef DEBUG_TP
							std::stringstream ss2;
							ss2 << m_sTabs << "E " << std::setw(3) << m_iFibStart << /*"= " << localRes <<*/ std::endl;
							std::cout << ss2.str();
#endif
		//_iResult = localRes;				// CANNOT: CONST OPERATOR 
		return localRes;
	}
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


ThreadPoolExample::ThreadPoolExample() :
	m_scheduler(N_THREADS_IN_POOL)
{}


void ThreadPoolExample::test(e_exec_mode mode, long N)
{
					const std::string STR_MODE = (mode == e_exec_mode::RUN_WAIT ? "WAIT  " : (mode == e_exec_mode::RUN_ALL_THEN_WAIT ? "ALL   " : "THRD  "));
					const auto t0 = tic();

	std::vector<std::thread> vt;

	for (long fibArg = 0; fibArg < N; fibArg++)
	{
		switch (mode)
		{
		case e_exec_mode::RUN_WAIT:
			m_group.run_and_wait(FibFunctor(fibArg));
			break;
		case e_exec_mode::RUN_ALL_THEN_WAIT:
			m_group.run(FibFunctor(fibArg));
			break;
		case e_exec_mode::THREADS:
			vt.push_back(std::thread(fibonacci_serial, fibArg));
			break;
		}
#ifdef DEBUG_TP
					std::stringstream ss;
					ss << "Spawned Fib(" << fibArg << ")" << std::endl;
					std::cout << ss.str();
#endif
	}
					// std::cout << "[" << STR_MODE << "]\tspawned all iterations" << std::endl;

	switch (mode)
	{
	case e_exec_mode::RUN_WAIT:
		break;
	case e_exec_mode::RUN_ALL_THEN_WAIT:
		m_group.wait();
		break;
	case e_exec_mode::THREADS:
		for (long i = 0; i < N; ++i)
		{
			vt.at(i).join();
		}
		break;
	}
					toclabel(t0, STR_MODE);
}
