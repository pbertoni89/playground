#include <iostream>
#include <atomic>
#include <mutex>
#include <future>
#include <vector>
#include <sstream>


using recv_t = uint8_t;
using tc_t = uint16_t;
std::mutex m;

std::pair<std::atomic<recv_t>, std::atomic<tc_t>> cache{0, 0};

constexpr recv_t BANG = UINT8_MAX;
constexpr tc_t BOOM = BANG;


template <typename T>
tc_t mistery(T * p)
{
	std::stringstream ss;
	tc_t rv = 0xFFFF;

	auto input = *(recv_t*) p;
	ss << +(*p) << ", " << +input << ", cache " << +cache.first << ", " << cache.second;

	{
		std::lock_guard<std::mutex> lock(m);
	
		if (input > cache.first + 1)
		{
			throw std::runtime_erro
		}
		
		if (cache.first == BANG and input < cache.first)
		{
			// input ++;
			cache.first = 0;
			cache.second ++;
			ss << ", overflow";
		}
		else
			cache.first = std::max(cache.first.load(), input);

		const tc_t TC_BASELINE = cache.second * BOOM;
		rv = TC_BASELINE + (tc_t) input;

		ss << " ---> " << rv << ", cache " << +cache.first << ", " << cache.second << "\n";
		std::cerr << ss.str();
	}
	return rv;
}


template <typename T>
tc_t mistery(const T & t)
{
	return mistery(&t);
}


int main(int argc, char ** argv)
{
	
	for (int i = 0; i < 600; i += 1)
	{
		mistery(i);
	}
	return 4;
	
	
	std::vector<std::pair<recv_t, std::future<tc_t>>> v;

	for (recv_t u = 0; u < 255; u ++)
	{
		v.emplace_back(u,
					   std::async(std::launch::async, [u] () { return mistery(u); }));
	}

	for (auto & p : v)
		p.second.get();

	auto x = 255;
	mistery(x);
	x = 256;
	const auto RV = mistery(x);
	std::cout << "final " << RV << "\n";

	for (int i = 0; i < 600; i ++)
	{
	//	mistery(i);
	}

	return 0;
}
