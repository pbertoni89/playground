// Source: https://devblogs.microsoft.com/cppblog/using-c17-parallel-algorithms-for-better-performance/

// compile with:
//	debug: cl /EHsc /W4 /WX /std:c++latest /Fedebug /MDd .\program.cpp
//	release: cl /EHsc /W4 /WX /std:c++latest /Ferelease /MD /O2 .\program.cpp
#include <stddef.h>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>
#include <iostream>
#include <execution>

using std::chrono::duration;
using std::chrono::duration_cast;
using hrc = std::chrono::high_resolution_clock;
using std::milli;
using std::random_device;
using std::sort;
using std::vector;
using t_d = float;

size_t testSize = 1'000'000;
int iterationCount = 5;


void print_results(const char * const tag, const vector<t_d>& sorted,
					hrc::time_point startTime,
					hrc::time_point endTime)
{
	printf("%s: Lowest: %g Highest: %g Time: %fms\n",
		tag,
		sorted.front(), sorted.back(),
		duration_cast<duration<double, milli>>(endTime - startTime).count());
}


int main(int argc, char ** argv)
{
	if (argc > 2)
	{
		testSize = std::stoi(argv[1]);
		iterationCount = std::stoi(argv[2]);
	}

	random_device rd;

	// generate some random numbers: WARNING really consuming !
	constexpr bool RANDOM_INIT = false;
	printf("Testing with %zu numbers for %i iterations...\n", testSize, iterationCount);
	vector<t_d> vd(testSize);
	if (RANDOM_INIT)
		for (auto & d : vd)
		{
			d = static_cast<t_d>(rd());
		}

	std::cout << "serials" << std::endl;
	// time how long it takes to sort them:
	for (int i = 0; i < iterationCount; ++i)
	{
		// std::cout << "serial " << i << std::endl;
		vector<t_d> sorted(vd);
		const auto startTime = hrc::now();
		sort(sorted.begin(), sorted.end());
		const auto endTime = hrc::now();
		print_results("Serial", sorted, startTime, endTime);
	}

	std::cout << "parallels" << std::endl;
	for (int i = 0; i < iterationCount; ++i)
	{
		// std::cout << "parallel " << i << std::endl;
		vector<t_d> sorted(vd);
		const auto startTime = hrc::now();
		// same sort call as above, but with par_unseq:
		sort(std::execution::par_unseq, sorted.begin(), sorted.end());
		const auto endTime = hrc::now();
		// in our output, note that these are the parallel results:
		print_results("Parallel", sorted, startTime, endTime);
	}
}
