#include "../include/Performances.hpp"

#include <iostream>


namespace pietra
{
	namespace utils
	{
		void Performances::add_tic(int lbl)
		{
			vtp.emplace_back(std::chrono::high_resolution_clock::now(), lbl);
		}


		void Performances::tictoc_results()
		{
			std::cout << "TIC-TOCS [us]";
			for (int t=0; t<vtp.size()-1; t++)
			{
				const auto TOC = vtp[t+1].first, TIC = vtp[t].first;
				const auto TOCLBL = vtp[t+1].second, TICLBL = vtp[t].second;
				std::cout << "\n\t" << t << " - lines " << TICLBL << " to " << TOCLBL
						<< " -> " << std::chrono::duration_cast<std::chrono::microseconds>(TOC-TIC).count();
			}
			std::cout << std::endl;
		}
	}
}
