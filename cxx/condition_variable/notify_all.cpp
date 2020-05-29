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
bool ready = false;
int processed = 0;


void worker_thread(int id)
{
	std::stringstream ss;
	ss << "w" << id << " started\n"; std::cerr << ss.str(); ss.str("");

	// Wait until main() sends data
	{
		std::unique_lock<std::mutex> lk(m);

		cv.wait(lk, [] { return ready; });

		// after the wait, we own the lock.
		ss << "w" << id << " is processing data\n"; std::cerr << ss.str(); ss.str("");

		std::this_thread::sleep_for(std::chrono::seconds(id));

		data += " <done_" + std::to_string(id) + ">";

		// Send data back to main()
		processed ++;
		ss << "w" << id << " signals data processing completed\n"; std::cerr << ss.str(); ss.str("");

		// Manual unlocking is done before notifying, to avoid waking up
		// the waiting thread only to block again (see notify_one for details)
		// lk.unlock();
	}
	cv.notify_one();
}


int main()
{
	std::stringstream ss;

	std::vector<std::thread> vworkers;
	for (int i=0; i<NTHREADS; i++)
		vworkers.emplace_back(worker_thread, i);

	//std::thread worker(worker_thread);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	data = "Example data";

	// send data to the worker thread
	{
		std::lock_guard<std::mutex> lk(m);
		ready = true;
		ss << "main() signals data ready for processing\n"; std::cerr << ss.str(); ss.str("");
	}
	cv.notify_all();

	// wait for the workers
	{
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [] { return processed == NTHREADS; });
	}

	ss << "main() merged threads, data = \"" << data << "\"\n"; std::cerr << ss.str(); ss.str("");

	for (auto & t : vworkers)
		t.join();
	// worker.join();
}
