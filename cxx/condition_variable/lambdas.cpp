#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>

std::condition_variable cv;
std::mutex m;


int main()
{
	int value = 100;
	bool notified = false;

	std::thread reporter([&]()
	{
		std::unique_lock<std::mutex> lock(m);
		while (not notified)
		{
			cv.wait(lock);
		}

		std::cout << "The value is " << value << std::endl;
	});

	std::thread assigner([&]()
	{
		value = 20;

		notified = true;
		cv.notify_one();
	});

	reporter.join();
	assigner.join();
	return 0;
}
