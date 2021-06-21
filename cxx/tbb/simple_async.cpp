
#include <tbb/task.h>
#include <tbb/task_group.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

#include <iostream>
#include <vector>
// #include <list>
#include <cstdlib>
#include <thread>


[[nodiscard]] long fib(long a)
{
	if (a < 2)
		return 1;

	return fib(a - 1) + fib(a - 2);
}


class PrintTask 
{
	const int m_i;
public:
	PrintTask(int i) : m_i(i) {}

	void operator() ()
	{
		std::cout << "hi world!: " <<  std::this_thread::get_id() << std::endl;

		std::cout << "got " << fib(50) << " from " <<  std::this_thread::get_id() << std::endl;
	}
};


#if 0
void demo1()
{     
	tbb::task_group group;

	for (int i = 0; i < 100; ++i)
	{
		group.run(PrintTask());
	}      

	group.wait();
}
#endif

void demo2()
{
	// tbb::task_scheduler_init init;  // Automatic number of threads
	// tbb::task_scheduler_init init(tbb::task_scheduler_init::default_num_threads());  // Explicit number of threads

	std::vector<PrintTask> tasks;
	for (int i=0;i<1000;++i)
	{
		tasks.push_back(PrintTask(i));
	}

	tbb::parallel_for(
		tbb::blocked_range<size_t>(0,tasks.size()),
		[&tasks](const tbb::blocked_range<size_t>& r)
		{
			for (size_t i=r.begin();i<r.end();++i)
			{
				tasks[i]();
			}
		}
	);

	std::cerr << std::endl;
}


int main(int argc, char** argv)
{
	demo2();
	return(0);
}
