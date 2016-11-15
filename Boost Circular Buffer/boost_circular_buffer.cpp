#include "boost_circular_buffer.hpp"

void CircularBufferExample::print_int()
{
	std::stringstream ss;
	//for (auto it = _cb_int.begin(); it != _cb_int.end(); it++)		// OK
	for (int i = 0; i < _cb_int.size(); i++)
	{
		//std::cout << "\t" << *it;
		ss << "\n\tV " << _cb_int[i] << "\t& " << &(_cb_int[i]);
	}
	std::cout << ss.str() << "\n\n";
}

void CircularBufferExample::print_p_int()
{
	std::stringstream ss;
	ss << "\n\tFRONT = RIGHTMOST IN CUBE";
	for (int i = 0; i < _cb_p_int.size(); i++)
	{
		ss << "\n\tV " << _cb_p_int[i] << "\t& " << &(_cb_p_int[i]) << "\t* " << *(_cb_p_int[i]);
	}
	ss << "\n\tBACK = LEFTMOST IN CUBE\n\n";
	std::cout << ss.str();
}

std::string CircularBufferExample::print_arr_int(const arr & a)
{
	std::stringstream ss;
	ss << "&a " << &a;
	for (int j = 0; j < CIRCULAR_BUFFER_SIZE; j++)
		ss << "\t" << a[j];
	return ss.str();
}

void CircularBufferExample::print_cube()
{
	std::stringstream ss;
	ss << "\n\tFRONT = RIGHTMOST IN CUBE";
	//for (auto itARR = _cb_arr_int.begin(); itARR != _cb_arr_int.end(); itARR++)
	for (int i = 0; i < _cb_arr_int.size(); i++)
	{
		ss << "\n" << print_arr_int(_cb_arr_int[i]) << " at " << &(_cb_arr_int[i]);
	}
	ss << "\n\tBACK = LEFTMOST IN CUBE\n\n";
	std::cout << ss.str();
}

void CircularBufferExample::print_p_arr_int()
{
	std::stringstream ss;
	ss << "\n\tFRONT = RIGHTMOST IN CUBE*";
	//for (auto itARR = _cb_arr_int.begin(); itARR != _cb_arr_int.end(); itARR++)
	for (int i = 0; i < _cb_p_arr_int.size(); i++)
	{
		ss << "\ngot " << _cb_p_arr_int[i] << "\tat " << &(_cb_p_arr_int[i]) << "\t" << print_arr_int(*(_cb_p_arr_int[i]));
	}
	ss << "\n\tBACK = LEFTMOST IN CUBE*\n\n";
	std::cout << ss.str();
}


CircularBufferExample::CircularBufferExample() :
	_cb_int(CIRCULAR_BUFFER_SIZE),
	_cb_p_int(CIRCULAR_BUFFER_SIZE),
	_cb_arr_int(CIRCULAR_BUFFER_SIZE),
	_cb_p_arr_int(CIRCULAR_BUFFER_SIZE),
	_cb_pline_tc(CIRCULAR_BUFFER_SIZE)
{
}

void CircularBufferExample::run_int()
{
	// Insert some elements into the buffer.
	_cb_int.push_back(1);
	_cb_int.push_back(2);
	_cb_int.push_back(3);

	int a = _cb_int[0];  // a == 1
	int b = _cb_int[1];  // b == 2
	int c = _cb_int[2];  // c == 3

					// The buffer is full now, pushing subsequent
					// elements will overwrite the front-most elements.

	_cb_int.push_back(4);  // Overwrite 1 with 4.
	_cb_int.push_back(5);  // Overwrite 2 with 5.

						// The buffer now contains 3, 4 and 5.

	a = _cb_int[0];  // a == 3
	b = _cb_int[1];  // b == 4
	c = _cb_int[2];  // c == 5

	print_int();

	// Elements can be popped from either the front or the back.
	_cb_int.pop_back();  // 5 is removed.
	_cb_int.pop_front(); // 3 is removed.

	print_int();

	int d = _cb_int[0];  // d == 4

	// END OF BOOST TUTORIAL

	_cb_int.pop_back();				// 4 is removed. CB is empty

	_cb_int.push_back(2);
	_cb_int.push_back(4);
	_cb_int.push_back(6);

	print_int();

	int iNew = 8;

	std::cout << "Pushing back " << iNew << std::endl;

	_cb_int.push_back(iNew);

	print_int();

	// I CANNOT DO THIS std::cout << "Last element pushed_back is by definition = " << *(_cb_int.end()) << std::endl;
	std::cout << "Last element pushed_back is by definition = " << _cb_int.at(_cb_int.size() - 1) << std::endl;
}

void CircularBufferExample::run_p_int()
{
	std::cout << "\n\n\n" << __FUNCDNAME__ << "\n";

	int ** ii = new int*[EXAMPLE_DATA];
	for (int i = 0; i < EXAMPLE_DATA; i++)
	{
		ii[i] = new int((i + 1) * 11);
		std::cout << "Allocated i" << i + 1 << "\t* " << *ii[i] << "\tV " << ii[i] << "\t& " << &ii[i] << "\n";

		if (i < EXAMPLE_DATA - 1)
			_cb_p_int.push_back(ii[i]);
	}

	print_p_int();

	_cb_p_int.push_back(ii[EXAMPLE_DATA - 1]);

	print_p_int();

	_cb_p_int.pop_front();

	print_p_int();

	*ii[EXAMPLE_DATA - 2] = 55;		// CB reflect change (as expected). This means that it will be corrupted too from external delete

	print_p_int();

	for (int i = 0; i < 10000; i++)	// memleak but we don't care. we want to roll over addresses...
	{
		_cb_p_int.push_back(new int(i));
	}

	print_p_int();

	// DELETE

	for (int i = 0; i<EXAMPLE_DATA; i++)
	{
		delete ii[i];
	}
	delete ii;
}

void CircularBufferExample::run_arr_int()
{
	std::cout << "\n\n\n" << __FUNCSIG__ << "\n";

	for (int i = 0; i < EXAMPLE_DATA; i++)
	{
		arr arrNew = { 1 * (i+1), 2 * (i + 1), 3 * (i + 1) };

		std::cout << i << "-th arrNew at " << &arrNew << "\n";
			
		// &arrNew is always the same, because arrNew lives in this for. This implies that an insertion into CB is a DEEP COPY

		if (i < EXAMPLE_DATA - 1)
			_cb_arr_int.push_back(arrNew);
	}

	print_cube();

	arr nextArr = { 1000, 2000, 3000 };
	arr nextArrPlusOne = { 1001, 2001, 3001 };
	arr nextArrPlusTwo = { 1002, 2002, 3002 };
	nextArr = nextArrPlusOne;
	_cb_arr_int.push_back(nextArr);

	print_cube();

	nextArr = nextArrPlusTwo;		// no effects in CB. This means that data got COPIED.

	print_cube();

	_cb_arr_int.pop_front();

	print_cube();

	arr zeroArr;
	_cb_arr_int.push_back(zeroArr);

	print_cube();

	arr frontArr = _cb_arr_int.front();
	zeroArr = frontArr;				// no effects in CB

	print_cube();

	_cb_arr_int.push_back(zeroArr);		// OK

	print_cube();
}

void CircularBufferExample::run_p_arr_int()
{
	std::cout << "\n\n\n" << __FUNCSIG__ << "\n";

	arr * pArrNew, * pArrDelete;

	for (int i = 0; i < EXAMPLE_DATA; i++)
	{
		pArrNew = new arr( { 1 * (i + 1), 2 * (i + 1), 3 * (i + 1)} );
		_cb_p_arr_int.push_back(pArrNew);

		std::cout << i << "-th arrNew: got " << pArrNew << " at " << &pArrNew << ". Now size is " << _cb_p_arr_int.size() << "\n";
	}

	std::cout << "cb_p_arr_int size is now " << _cb_p_arr_int.size() << "\n";

	for (int i = 0; i < _cb_p_arr_int.size(); i++)
	{
		pArrDelete = _cb_p_arr_int.front();
		std::cout << i << "-th arrDelete FRONT: got " << pArrDelete << " at " << &pArrDelete << ". Now size is " << _cb_p_arr_int.size() << "\n";
		_cb_p_arr_int.pop_front();
		std::cout << i << "-th arrDelete POP: got " << pArrDelete << " at " << &pArrDelete << ". Now size is " << _cb_p_arr_int.size() << "\n";
		delete pArrDelete;
		std::cout << i << "-th arrDelete DELETE: Now size is " << _cb_p_arr_int.size() << "\n";

	}
}

void CircularBufferExample::run_performance()
{
	std::cout << "PHASE 1"; system("PAUSE");
	auto t0 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < EXAMPLE_DATA; i++)
	{
		arr arrNew;
		_cb_arr_int.push_back(arrNew);
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	std::cout << "PHASE 2"; system("PAUSE");
	auto t2 = std::chrono::high_resolution_clock::now();

	arr * pArrNew;
	for (int i = 0; i < EXAMPLE_DATA; i++)
	{
		pArrNew = new arr;
		_cb_p_arr_int.push_back(pArrNew);
	}

	auto t3 = std::chrono::high_resolution_clock::now();

	std::cout << "PHASE 3"; system("PAUSE");
	_cb_arr_int.clear();
	std::cout << "PHASE 4"; system("PAUSE");
	_cb_p_arr_int.clear();
	std::cout << "PHASE 5"; system("PAUSE");

	auto dt1 = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
	auto dt2 = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count();

	std::cout << dt1 << "\n" << dt2 << std::endl;
}

void CircularBufferExample::run_line_pline_comparison()
{
	system("PAUSE");

	pline_tc outerSource;

	for (int i = 0; i < EXAMPLE_DATA; i++)
	{
		_cb_pline_tc.push_back(pline_tc(i));
	}

/*	for (int i = 0; i < EXAMPLE_DATA; i++)
	{
		_cb_pline_tc.pop_front();
	}
*/
}

// ------------------------- PRINT ----------------------------------------