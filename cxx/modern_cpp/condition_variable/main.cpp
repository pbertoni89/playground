/*
	http://en.cppreference.com/w/cpp/thread/condition_variable
*/

#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <condition_variable>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip>


std::mutex m;
std::condition_variable cv;

// Even if the shared variable is atomic, it must be modified under the mutex in order to correctly publish the modification to the waiting thread
bool ready = false;

std::string data;

bool processed = false;



class t_timestamp
{
public:
	uint16_t year, month, day, hour, minute, second, milli;

	t_timestamp()
	{
		const boost::posix_time::ptime ptNow(boost::posix_time::microsec_clock::local_time());
		const boost::posix_time::time_duration td = ptNow.time_of_day();

		year = ptNow.date().year();
		month = static_cast<uint16_t>(ptNow.date().month());
		day = ptNow.date().day();
		hour = ptNow.time_of_day().hours();
		minute = ptNow.time_of_day().minutes();
		second = ptNow.time_of_day().seconds();
		milli = (uint16_t) (td.fractional_seconds() / 1000);
	}

	std::string to_string(bool millis = true, bool date = false) const
	{
		std::stringstream ss;
		if (date)
			ss << year << "_" << std::setfill('0') << std::setw(2) << month << "_" << std::setfill('0') << std::setw(2) << day << "__";
		ss << std::setfill('0') << std::setw(2) << hour << "_" << std::setfill('0') << std::setw(2) << minute << "_" << std::setfill('0') << std::setw(2) << second;
		if (millis)
			ss << "__" << std::setfill('0') << std::setw(3) << milli;
		return ss.str();
	}
};



void worker_thread (int id)
{
	std::cerr << t_timestamp().to_string() << " w " << id << " starts living\n";

	// Wait until main() sends data
	std::unique_lock<std::mutex> lk(m);
	cv.wait(lk, [] {return ready;} );

	// after the wait, we own the lock.
	std::cerr << t_timestamp().to_string() << " w " << id << " processes data\n";
	data += ", ap_" + std::to_string(id);

	std::this_thread::sleep_for(std::chrono::seconds(id));

	// Send data back to main()
	processed = true;
	std::cerr << t_timestamp().to_string() << " w " << id << " signals completion\n";

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lk.unlock();
	cv.notify_one();
}


int main()
{
	std::vector<std::thread> vt;
	for (int i = 0; i < 5; i++)
		vt.emplace_back(worker_thread, i);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	
					data = "Example data";
					// send data to the worker thread
					{
						std::lock_guard<std::mutex> lk(m);
						std::cerr << t_timestamp().to_string() << " main() signals data ready for processing\n";
						ready = true;
					}
					cv.notify_one();

					// wait(_for|_until) for the worker
					{
						std::unique_lock<std::mutex> lk(m);
						cv.wait(lk, [] {return processed;} );
					}
					std::cerr << t_timestamp().to_string() << " main() is back, data = " << data << '\n';

	for (auto & t : vt)
		t.join();
}

