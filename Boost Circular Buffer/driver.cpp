#include "boost_circular_buffer.hpp"

int main(int argc, char* argv[])
{
	CircularBufferExample cbe;

#ifdef TEST_PERFORMANCE
	cbe.run_performance();
#else
	cbe.run_int();

	cbe.run_p_int();

	cbe.run_arr_int();

	cbe.run_p_arr_int();

	cbe.run_line_pline_comparison();
#endif

	return 0;
}
