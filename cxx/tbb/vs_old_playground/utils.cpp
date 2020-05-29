#include "utils.hpp"

#include <iostream>


t_tp tic()
{
	return std::chrono::high_resolution_clock::now();
}


double toc(t_tp _tic)
{
	return tictoc(_tic, tic());
}

double tictoc(t_tp _tic, t_tp _toc)
{
	return (1.e-9) * std::chrono::duration_cast<std::chrono::nanoseconds>(_toc - _tic).count();
}


void toclabel(t_tp tic1, std::string lbl)
{
	std::cout << "[" << lbl << "]\t" << std::scientific << toc(tic1) << std::endl;
}


long fibonacci_serial(long n)
{
    if (n<2)
	{
        return n;
	}
    else
	{
        return fibonacci_serial(n-1) + fibonacci_serial(n-2);
	}
}
