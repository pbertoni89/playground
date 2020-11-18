#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <condition_variable>

// condition_variable is used in combination with a std::mutex to facilitate inter-thread communication.
std::mutex m;
std::condition_variable cv;

constexpr int NTHREADS = 4;

// Even if the shared variable is atomic, it must be modified under the mutex in order to correctly publish the modification to the waiting thread
std::string data;
bool bReady = false;
int iFinish = 0;


void worker_thread(int id)
{
	std::stringstream ss;
	ss << "[w" << id << "] started\n"; std::cerr << ss.str(); ss.str("");

	// Wait until main() sends data
	{
		std::unique_lock<std::mutex> lk(m);

		cv.wait(lk, [] { return bReady; });

		// after the wait, we own the lock.
		ss << "[w" << id << "] is processing data\n"; std::cerr << ss.str(); ss.str("");

		std::this_thread::sleep_for(std::chrono::seconds(id));

		data += " <done_" + std::to_string(id) + ">";

		// Send data back to main()
		iFinish ++;
		ss << "[w" << id << "] signals data processing completed\n"; std::cerr << ss.str(); ss.str("");

		// Manual unlocking is done before notifying, to avoid waking up
		// the waiting thread only to block again (see notify_one for details)
		// lk.unlock();
	}
	cv.notify_one();
}


void demo()
{
	std::stringstream ss;
	std::vector<std::thread> vt;
	for (int i=0; i<NTHREADS; i++)
		vt.emplace_back(worker_thread, i);

	std::this_thread::sleep_for(std::chrono::seconds(1));  // make sure all thread are waiting for cv
	data = "Example data";

	// send data to the worker thread
	{
		std::lock_guard<std::mutex> lk(m);
		bReady = true;
		ss << "[main] signals data ready for processing\n"; std::cerr << ss.str(); ss.str("");
	}
	cv.notify_all();  // outta mutex, or deadlocks!

	// wait for the workers
	{
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [] { return iFinish % NTHREADS == 0; });
	}

	ss << "[main] merged " << NTHREADS << " threads, data = \"" << data << "\"\n"; std::cerr << ss.str(); ss.str("");

	for (auto & t : vt)
		t.join();
}

int main()
{
	while (true)
		demo();
}
