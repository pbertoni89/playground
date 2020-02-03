#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <iterator>
#include <numeric>



#include <vector>
#include <string>
#include <numeric>
#include <chrono>
#include <memory>
#include <cmath>

constexpr bool PRINT = true;

using t_tp = std::chrono::time_point<std::chrono::high_resolution_clock>;

t_tp tic() noexcept
{
	return std::chrono::high_resolution_clock::now();
}

template<typename T>
decltype(auto)
tictoc(t_tp t0, t_tp t1) noexcept
{
	if (t0 > t1)
	{
		const t_tp temp =t0;
		t0 = t1;
		t1 = temp;
	}
	return std::chrono::duration_cast<T>(t1 - t0).count();
}

template<typename T>
decltype(auto)
tictoc(t_tp t0) noexcept
{
	return tictoc<T>(t0, tic());
}


template<typename T>
void printv(const std::vector<T> & V, const std::string & LBL)
{
	constexpr size_t EXCERPT = 4;
	std::cout << LBL << " contains: ";
	// TOO LONG std::copy(V.begin(), V.end(), std::ostream_iterator<T>(std::cout, " "));
	for (auto i=0; i<EXCERPT; i++)
		std::cout << " " << V[i];
	std::cout << "...";
	for (auto i=V.size()-EXCERPT; i<V.size(); i++)
		std::cout << " " << V[i];
	std::cout << '\n';
}


int main(int argc, char ** argv)
{
	const size_t N = std::atoi(argv[1]);
	const bool MOVE = std::atoi(argv[2]) == 1;

	std::vector<int> vfrom(N);
	std::iota(vfrom.begin(), vfrom.end(), 0);

	std::vector<int> vto;
	vto.reserve(N);

	const auto t0 = tic();
	if (MOVE)
		std::move(vfrom.begin(), vfrom.end(), std::back_inserter(vto));
	else
		std::copy(vfrom.begin(), vfrom.end(), std::back_inserter(vto));

	std::cout << "action performed in [us] " << tictoc<std::chrono::microseconds>(t0) << "\n";

// or, alternatively,
//  std::vector<int> vto(vfrom.size());
//  std::copy(vfrom.begin(), vfrom.end(), vto.begin());
// either way is equivalent to
//  std::vector<int> vto = vfrom;

	if (PRINT)
	{
		printv(vfrom, "vfrom");
		printv(vto, "vto");
	}
}
