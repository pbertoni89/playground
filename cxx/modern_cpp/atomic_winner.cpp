
// constructing atomics
#include <iostream>       // std::cout
#include <atomic>         // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>         // std::thread, std::this_thread::yield
#include <vector>         // std::vector


std::atomic<bool> ready(false);

std::atomic_flag winner = ATOMIC_FLAG_INIT;


using tci_t = std::pair<int, int>;

std::atomic<tci_t> atci;


void count1m(int id)
{
	while (!ready)
	{
		// wait for the ready signal
		std::this_thread::yield();
	}

	// go!, count to 1 million
	for (volatile int i = 0; i<1000000; ++i)
	{
	}

	if (!winner.test_and_set())
	{
		std::cout << "thread #" << id << " won!" << std::endl;
	}
};


int main()
{
	std::vector<std::thread> threads;
	std::cout << "spawning 10 threads that count to 1 million..." << std::endl;

	for (int i = 1; i <= 10; ++i)
	{
		threads.push_back(std::thread(count1m, i));
	}

	ready = true;

	for (auto& th : threads)
	{
		th.join();
	}

	return 0;
}
