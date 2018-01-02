#include <chrono>
#include <vector>


namespace pietra
{
	namespace utils
	{
		class Performances
		{
			std::vector<std::pair<std::chrono::time_point<std::chrono::high_resolution_clock>, int>> vtp;

		public:

			void add_tic(int lbl);
			void tictoc_results();
		};


		#define ADD_TIC performances.add_tic(__LINE__);
	}
}
