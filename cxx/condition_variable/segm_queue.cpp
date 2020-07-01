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


// std::mutex m;
std::condition_variable cv;


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



void thread_producer (int id)
{
	std::cerr << t_timestamp().to_string() << " w " << id << " starts living\n";

	while (true)
	{
		{
//			std::unique_lock<std::mutex> lk(m);
			std::cerr << t_timestamp().to_string() << " w " << id << " locked\n";

//			lk.unlock();
			std::this_thread::sleep_for(std::chrono::seconds(id ? 20 : 5));
		}
		cv.notify_one();
	}
}


int main()
{
	std::vector<std::thread> vt;
	for (int i = 0; i < 4; i++)
		vt.emplace_back(thread_producer, i);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (true)
	{
		{
			std::mutex m;
			std::unique_lock<std::mutex> lk(m);
			std::cerr << t_timestamp().to_string() << " main locked\n";
			cv.wait(lk);
			lk.unlock();
		}
		std::cerr << t_timestamp().to_string() << " main waited\n";
	}

	for (auto & t : vt)
		t.join();
}

