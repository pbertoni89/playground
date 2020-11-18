#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>
#include <condition_variable>

using namespace std::chrono_literals;

// condition_variable is used in combination with a std::mutex to facilitate inter-thread communication.
std::mutex m;
std::condition_variable cv;

constexpr int NTHREADS = 4;

bool bReady = false;
std::atomic<int> aiFinish;

// What differs from notify_all ???? We reduced the critical region to unleash concurrent requests (cfr Controllers)


void worker_thread(int id)
{
	std::stringstream ss;
	ss << "[w" << id << "] started\n"; std::cerr << ss.str(); ss.str("");

	// Wait until main() sends data
	{
		std::unique_lock<std::mutex> lock(m);

		cv.wait(lock, [] { return bReady; });
		// Manual unlocking is done before notifying, to avoid waking up
		// the waiting thread only to block again (see notify_one for details)
		// lock.unlock();
	}
	cv.notify_one();

	// after the wait, we own the lock.
	const size_t MS = 500 + 500 * id;
	ss << "[w" << id << "] is sleeping for " << MS << "\n"; std::cerr << ss.str(); ss.str("");

	std::this_thread::sleep_for(std::chrono::milliseconds(MS));

	ss << "[w" << id << "] signals data processing completed " << aiFinish << "\n"; std::cerr << ss.str(); ss.str("");
	aiFinish ++;
}


void demo()
{
	std::this_thread::sleep_for(10s);  // make sure stdout flushes
	std::cerr << "\n#  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #\n\n";
	std::stringstream ss;
	std::vector<std::thread> vt;
	for (int i=0; i<NTHREADS; i++)
		vt.emplace_back(worker_thread, i);

	std::this_thread::sleep_for(1s);  // make sure all thread are waiting for cv

	// send data to the worker thread
	{
		std::lock_guard<std::mutex> lock(m);
		bReady = true;
		ss << "[main] signals data ready for processing\n"; std::cerr << ss.str(); ss.str("");
	}
	cv.notify_all();  // outta mutex, or deadlocks!

	// wait for the workers
	{
		std::unique_lock<std::mutex> lock(m);

		while (not cv.wait_for(lock, 1s, []
		{
			std::cerr << "[main] waiting " << aiFinish.load() << "\n";
			return aiFinish.load() % NTHREADS == 0;
		}));
	}

	ss << "[main] merged " << aiFinish << "/" << NTHREADS << " threads\n"; std::cerr << ss.str(); ss.str("");

	for (auto & t : vt)
		t.join();
}


int main()
{
	aiFinish = 0;
	while (true)
		demo();
}
