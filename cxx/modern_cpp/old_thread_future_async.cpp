/* http://stackoverflow.com/questions/9094422/how-to-check-if-a-stdthread-is-still-running */

#include "stdafx.h"

#include <future>
#include <thread>
#include <sstream>
#include <chrono>
#include <iostream>
#include <atomic>

#define SECS_SLEEP 2
#define SECS_SLEEP_MAJOR 3


void _thread_fun(int exSrc)
{
	std::cout << std::this_thread::get_id() << "\tsleeping for " << SECS_SLEEP << "\tstart: must set " << exSrc << std::endl;

	static int _exSrc = exSrc;

	std::this_thread::sleep_for(std::chrono::seconds(SECS_SLEEP));
	std::cout << std::this_thread::get_id() << "\tsleeping for " << SECS_SLEEP << "\tend  : had  set " << _exSrc << std::endl;
}


void _print_status(int ex, int time, std::future_status status)
{
	std::stringstream ss;
	ss << "ex" << ex << ":" << time << ": Thread ";

	switch (status)
	{
	case std::future_status::ready:
		ss << "ready (finished)";
		break;
	case std::future_status::deferred:
		ss << "deferred (still running)";
		break;
	case std::future_status::timeout:
		ss << "timeout (still running)";
		break;
	}

	std::cout << ss.str() << std::endl;
}


/**
 * If you are willing to make use of C++11 std::async and std::future for running your tasks,
 * then you can utilize the wait_for function of std::future to check if the thread is still
 * running in a neat way like this
 */
int ex1()
{
	/* Run some task on new thread. The launch policy std::launch::async
	makes sure that the task is run asynchronously on a new thread */
	auto future = std::async(std::launch::async, []
	{
		_thread_fun(1);
		return 8;
	});

	auto status = future.wait_for(std::chrono::milliseconds(0));

	_print_status(1, 1, status);							// Still running ofc

	//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

	std::this_thread::sleep_for(std::chrono::seconds(SECS_SLEEP_MAJOR));
	status = future.wait_for(std::chrono::milliseconds(0));
	_print_status(1, 2, status);							//

	//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

	// future.get() is a BLOCKING CALL. It lasts for almost 2 seconds = duration of _thread_fun(), if previous block is commented.
	// Otherwise, it would "steal" time from this wait

	auto tic = std::chrono::high_resolution_clock::now();

	auto result = future.get();

	auto toc = std::chrono::high_resolution_clock::now();

	std::cout << "ex1: future.get() = " << result << " ran in "
		<< std::chrono::duration_cast<std::chrono::nanoseconds>(toc-tic).count() << std::endl;

	//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

	// WILL THROW: FUTURE IS "FINISHED" YET, since I waited for its termination !
	try
	{
		status = future.wait_for(std::chrono::milliseconds(0));
		_print_status(1, 3, status);
	}
	catch (std::exception& exc)
	{
		std::cout << "EXCEPTION CAUGHT: " << exc.what() << std::endl;
	}

	return result;
}


/** If you must use std::thread then you can use std::promise to get a future object							*/
void ex2()
{
	// Create a promise and get its future
	std::promise<bool> p;
	auto future = p.get_future();

	// Run some task on a new thread
	std::thread t([&p]
	{
		_thread_fun(2);
		p.set_value(true); // done atomically
	});

	auto status = future.wait_for(std::chrono::milliseconds(0));

	_print_status(2, 1, status);

	t.join();
}


/** it might be simpler to just do it like others have already mentioned										*/
void ex3()
{
	std::atomic<bool> done(false); // Use an atomic flag.

	// Run some task on a new thread. Make sure to set the done flag to true when finished
	std::thread t([&done]
	{
		_thread_fun(3);
		done = true;
	});

	if (done)
		std::cout << "Thread finished" << std::endl;
	else
		std::cout << "Thread still running" << std::endl;

	t.join();
}


/** There's also the std::packaged_task for use with std::thread for a cleaner solution than using std::promise	*/
void ex4()
{
	// Create a packaged_task using some task and get its future
	std::packaged_task<void()> task([]
	{
		_thread_fun(4);
	});

	auto future = task.get_future();

	// Run task on new thread
	std::thread t(std::move(task));

	// Get thread status using wait_for as before
	auto status = future.wait_for(std::chrono::milliseconds(0));

	_print_status(4, 1, status);

	t.join();
}


int main(int argc, char** argv)
{
	ex1();
	ex2();

	/** Both of these examples will output:
			Thread still running
		This is of course because the thread status is checked before the task is finished		*/

	ex3();
	ex4();
}
