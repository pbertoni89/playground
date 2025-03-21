#pragma once

#include <chrono>
#include <string>


using t_tp = std::chrono::time_point<std::chrono::high_resolution_clock>;

t_tp tic();

/**
 * @return elapsed seconds
 */
double toc(t_tp tic);

/**
 * @param _tic
 * @param _toc
 * @return elapsed seconds
 */
double tictoc(t_tp _tic, t_tp _toc);

/**
 * 
 * @param tic
 * @param lbl
 */
void toclabel(t_tp tic, std::string lbl);

/**
 * Fibonacci number
 * @param n
 * @return 
 */
long fibonacci_serial(long n);
