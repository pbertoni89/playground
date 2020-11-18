/* http://stackoverflow.com/questions/9094422/how-to-check-if-a-stdthread-is-still-running */

#include <future>
#include <thread>
#include <sstream>
#include <chrono>
#include <iostream>
#include <atomic>

using namespace std::chrono_literals;


void thread_fun(int exSrc)
{
	static int _exSrc = -1;
	std::cout << std::this_thread::get_id() << "\tstart " << _exSrc << " <- " << exSrc << std::endl;
	std::this_thread::sleep_for(2s);
	_exSrc = exSrc;
	std::cout << std::this_thread::get_id() << "\tend   " << _exSrc << " <- " << exSrc << std::endl;
}


/**
 * https://en.cppreference.com/w/cpp/thread/future/wait_for
 */
void print_status(std::future_status status)
{
	std::cout << "status ";

	switch (status)
	{
	case std::future_status::ready:
		std::cout << "ready (finished)";
		break;
	case std::future_status::deferred:
		std::cout << "deferred (not stated yet)";
		break;
	case std::future_status::timeout:
		std::cout << "timeout (still running)";
		break;
	default:
		std::cout << "unknown";
	}
	std::cout << std::endl;
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
		thread_fun(1);
		return 8;
	});

	auto status = future.wait_for(20ms);
	print_status(status);							// Still running ofc

	std::this_thread::sleep_for(3s);
	status = future.wait_for(0ms);
	print_status(status);							// Should have finished

	//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

	// future.get() is a BLOCKING CALL. It lasts for almost 2 seconds = duration of thread_fun(), if previous block is commented.
	// Otherwise, it would "steal" time from this wait

	auto tic = std::chrono::high_resolution_clock::now();

	auto result = future.get();

	auto toc = std::chrono::high_resolution_clock::now();

	std::cout << "ex 1: future.get() = " << result << " ran in "
		<< std::chrono::duration_cast<std::chrono::microseconds>(toc-tic).count() << " us" << std::endl;

	//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

	// WILL THROW: FUTURE IS "FINISHED" YET, since I waited for its termination !
	try
	{
		status = future.wait_for(0ms);
		print_status(status);
	}
	catch (const std::exception & exc)
	{
		std::cerr << "CAUGHT: " << exc.what() << std::endl;
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
		thread_fun(2);
		p.set_value(true); // done atomically
	});

	auto status = future.wait_for(0ms);

	print_status(status);

	t.join();
}


/** it might be simpler to just do it like others have already mentioned										*/
void ex3()
{
	std::atomic<bool> done(false); // Use an atomic flag.

	// Run some task on a new thread. Make sure to set the done flag to true when finished
	std::thread t([&done]
	{
		thread_fun(3);
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
		thread_fun(4);
	});

	auto future = task.get_future();

	// Run task on new thread
	std::thread t(std::move(task));

	// Get thread status using wait_for as before
	auto status = future.wait_for(0ms);

	print_status(status);

	t.join();
}


void ex5()
{
	std::future<int> f1;
	std::cout << "getting a " << (f1.valid() ? "" : "in") << "valid future..." << std::endl;
	try
	{
		f1.get();
		std::cout << "got an invalid future" << std::endl;
	}
	catch (const std::future_error & FE)
	{
		std::cerr << "CAUGHT: " << FE.what() << std::endl;
	}

	std::future<int> f2 = std::async(std::launch::deferred, []
	{
		std::this_thread::sleep_for(2s);
		return 8;
	});

	std::cout << "getting a " << (f2.valid() ? "" : "in") << "valid future..." << std::endl;
	f2.get();
	std::cout << "got an invalid future" << std::endl;
}


int main(int argc, char** argv)
{
	std::cout << "\n\n";
	ex1(); std::cout << "\n\n";
	ex2(); std::cout << "\n\n";

	/** Both of these examples will output:
			Thread still running
		This is of course because the thread status is checked before the task is finished		*/

	ex3(); std::cout << "\n\n";

	ex4(); std::cout << "\n\n";

	ex5(); std::cout << "\n\n";
}
