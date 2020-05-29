#include "Fibonacci.hpp"

#include "utils.hpp"

#include <tbb/task_group.h>
#include <tbb/task.h>


// #define DBG_TBB

#ifdef DBG_TBB
	#include <iostream>
	#include <iomanip>
	#include <sstream>
	#include <mutex>

	/**
	 * https://stackoverflow.com/questions/9110487/undefined-reference-to-a-static-member
	 */
	std::mutex __mtx;

	using t_lk = std::lock_guard<std::mutex>;
#endif


/**
 * Block parallel execution
 */
const int FIB_CUTOFF = 16;


long fibonacci_task_tree(long n)
{
	if (n < 2)
	{
		return n;
	}
	else
	{
		if (n < FIB_CUTOFF)
		{
			return fibonacci_serial(n);
		}
		else
		{
			long x, y;
			tbb::task_group g;
			g.run([&] {x = fibonacci_task_tree(n - 1); });	// spawn a task
			g.run([&] {y = fibonacci_task_tree(n - 2); });	// spawn another task
			g.wait();										// wait for both tasks to complete
			return x + y;
		}
	}
}


class FibTask :
	public tbb::task
{
	/**
	 * Argument of this task
	 */
	const long m_n;

	/**
	 * Reduction variable
	 */
	long * const m_sum;

	/**
	 * Call depth - only for logging purposes
	 */
	int m_lvl;

	/**
	 * Format output - only for logging purposes
	 */
	std::string m_tabs;

public:

	/**
	 * Format output - only for logging purposes
	 */
	static int __id;

	/**
	 * Construct a new derived tbb Task
	 * @param n
	 * @param sum
	 * @param lvl
	 */
	FibTask(long n, long * sum, int lvl = 0) :
		m_n(n), m_sum(sum), m_lvl(lvl), m_tabs("")
	{
#ifdef DBG_TBB
		std::stringstream ss;

		{
			t_lk lk(__mtx);

			ss << std::setw(3) << __id;

			for (int i=0; i<m_lvl; i++)
			{
				ss << "  ";
			}
			m_tabs = ss.str();

			__id ++;
		}
#endif
	}

	/**
	 * Overrides virtual function task::execute
	 *
	 * At first glance, the parallelism might appear to be limited, because the
	 * task creates only two child tasks. The trick here is recursive parallelism.
	 * The two child tasks each create two child tasks, and so on,
	 * until n <lt> Cutoff. This chain reaction creates a lot of potential parallelism.
	 *
	 * The advantage of the task scheduler is that it turns this potential parallelism
	 * into real parallelism in a very efficient way, because it chooses tasks
	 * to run in a way that keeps physical threads busy with relatively little context switching.
	 *
	 * @return either NULL, or a pointer to the next task to run.
	 * For more information on the non-NULL case see Scheduler Bypass.
	 */
	task* execute()
	{
		if (m_n < FIB_CUTOFF)
		{
			*m_sum = fibonacci_serial(m_n);
#ifdef DBG_TBB
			{
				t_lk lk(__mtx);
				std::cout << m_tabs << "F(" << m_n << ") <- "<< (*m_sum) << std::endl;
			}
#endif
		}
		else
		{
			long x, y;

			/*
			 * allocate space for the task. Remember that the top-level routine
			 * fibonacci_task used allocate_root() to allocate space for a task.
			 * The difference is that here the task is creating child tasks
			 */
			FibTask& a = * new(allocate_child()) FibTask(m_n-1, &x, m_lvl+1);
			FibTask& b = * new(allocate_child()) FibTask(m_n-2, &y, m_lvl+1);

#ifdef DBG_TBB
			{
				t_lk lk(__mtx);
				std::cout << m_tabs << "F(" << m_n << ") -> F(" << (m_n-1) << "), F(" << (m_n-2) << ")" << std::endl;
			}
#endif
			/*
			 * Set ref_count to 'two children plus one for the wait'
			 * this protects the successor from being implicitly pushed
			 */
			set_ref_count(3);

			// Start b running; return immediately
			spawn(b);

			/*
			 * Start a running and wait for all children (a and b)
			 * Remember to call before spawning any children:
			 * Failure to do so results in undefined behavior
			 */
			spawn_and_wait_for_all(a);

			*m_sum = x + y;

#ifdef DBG_TBB
			{
				t_lk lk(__mtx);
				std::cout << m_tabs << "F(" << m_n << ") <- " << x << " + " << y << " = " << (*m_sum) << std::endl;
			}
#endif
		}
		return nullptr;
	}
};


int FibTask::__id = 0;


long fibonacci_task(long n)
{
	long sum;

	/*
	 * Allocate space for the task. This is done by a special "overloaded new" and method task::allocate_root.
	 * The _root suffix in the name denotes the fact that the task created has no parent.
	 * It is the root of a task tree.
	 * Tasks must be allocated by special methods so that the space can be efficiently recycled when the task completes.
	 *
	 * Construct the task with the constructor FibTask(n, &sum) invoked by new.
	 * When the task is run in step 3, it computes the nth Fibonacci number and stores it into *sum.
	 */
	FibTask & a = * new(tbb::task::allocate_root()) FibTask(n, &sum);

	// Run the task to completion
	tbb::task::spawn_root_and_wait(a);

	return sum;
}