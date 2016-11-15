#pragma once

// THIS AVOID THE USAGE OF -D_SCL_SECURE_NO_WARNINGS.... NO, THEY DON'T WORK

#include <boost/circular_buffer.hpp>

#include <sstream>
#include <iostream>
#include <array>
#include <chrono>

//#define TEST_PERFORMANCE

#ifdef TEST_PERFORMANCE
#define CIRCULAR_BUFFER_SIZE 1024
#define ARRAY_SIZE 128*32
#define EXAMPLE_DATA 1000000
#else
#define CIRCULAR_BUFFER_SIZE 3
#define ARRAY_SIZE 5
#define EXAMPLE_DATA 4
#define XSPECTRA_N_BINS 3
#define XSPECTRA_N_PIXELS 4
#endif

typedef std::array<int, ARRAY_SIZE> arr;

typedef std::array<int, XSPECTRA_N_BINS> spectra;
typedef std::array<spectra, XSPECTRA_N_PIXELS> line;

typedef struct line_tc
{
	line _line;
	int _tc;

	line_tc(line line, int tc)
	{
		_line = line;
		_tc = tc;
	}

	line_tc(line * pLine, int tc)
	{
		_line = *pLine;
		_tc = tc;
	}

	~line_tc()
	{
		std::cout << "Deleting line_tc " << _tc << "\n";
	}

} line_tc;

typedef struct pline_tc
{
	line * _pLine;
	int _tc;

	pline_tc() :
		pline_tc(-1)
	{
		std::cout << "Default constructor has: " << _tc;
	}

	pline_tc(int tc)
	{
		std::stringstream ss;

		_tc = tc;
		_pLine = new line;

		for (int px = 0; px < XSPECTRA_N_PIXELS; px++)
			for (int bin = 0; bin < XSPECTRA_N_BINS; bin++)
				(*_pLine)[px][bin] = _tc;

		ss << "New line " << _tc << " at " << _pLine << "\n";

		for (int px = 0; px < XSPECTRA_N_PIXELS; px++)
		{
			ss << "\n\t";
			for (int bin = 0; bin < XSPECTRA_N_BINS; bin++)
			{
				ss << "\t" << (*_pLine)[px][bin];
			}
		}

		std::cout << ss.str() << "\n";
	}

	pline_tc(line line, int tc)
	{
		_pLine = &line;
		_tc = tc;
	}

	pline_tc(line * pLine, int tc)
	{
		_pLine = pLine;
		_tc = tc;
	}

	~pline_tc()
	{
		std::cout << "Delete line " << _tc << " at " << _pLine << "\n";
		delete _pLine;
	}

} pline_tc;

typedef boost::circular_buffer<line_tc> cube_line_tc;
typedef boost::circular_buffer<pline_tc> cube_pline_tc;

class CircularBufferExample
{
	boost::circular_buffer<int/*, std::allocator<int>*/> _cb_int;

	boost::circular_buffer<int*> _cb_p_int;

	boost::circular_buffer<arr> _cb_arr_int;

	boost::circular_buffer<arr*> _cb_p_arr_int;

	cube_pline_tc _cb_pline_tc;

	void print_int();

	void print_p_int();

	std::string print_arr_int(const arr & a);

	void print_cube();

	void print_p_arr_int();

public:

	CircularBufferExample();

	void run_int();

	void run_p_int();

	void run_arr_int();

	void run_p_arr_int();

	void run_line_pline_comparison();

	void run_performance();
};
