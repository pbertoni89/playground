#pragma once

// introduces std::thread
#include <tbb/compat/thread>


bool thr_foo(int something)
{
	std::cout << "START: something is " << something << std::endl;

	for (int i = 0; i < 20000000; i++)
	{
		something ++;
	}

	usleep(1);
	std::cout << "END: something is " << something << std::endl;

	return true;
}


void simple_threads()
{
	tbb::tbb_thread pMyThread1(thr_foo, 1);
	std::thread araberara1(thr_foo, 11);
	tbb::tbb_thread pMyThread2(thr_foo, 2);
	std::thread araberara2(thr_foo, 12);
	tbb::tbb_thread pMyThread3(thr_foo, 3);
	std::thread araberara3(thr_foo, 13);
	tbb::tbb_thread pMyThread4(thr_foo, 4);
	std::thread araberara4(thr_foo, 14);

	pMyThread1.join();
	pMyThread2.join();
	pMyThread3.join();
	pMyThread4.join();

	araberara1.join();
	araberara2.join();
	araberara3.join();
	araberara4.join();
}