#include "utils.hpp"

#include <iostream>


t_tp tic()
{
	return std::chrono::high_resolution_clock::now();
}


double toc(t_tp tic1)
{
	return (1.e-9) * std::chrono::duration_cast<std::chrono::nanoseconds>(tic() - tic1).count();
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
