// StdArrayPlayground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <array>
#include <iostream>

typedef std::array<int, 10> myarr;

typedef struct mystruct
{
	myarr arr;

	mystruct(myarr a) :
		arr(a)
	{
	}

	mystruct(myarr const & a) :
		arr(a)
	{
	}
} mystruct;


void print_array(myarr const & a)
{
	for (int i=0; i<a.size(); i++)
	{
		std::cout << "\t" << a[i];
	}
	std::cout << std::endl;
}


struct S {};

struct T
{
	T(S& s) : s{ s } {}

	S& s;
};

int main()
{

	myarr a1{4};
	myarr a2(a1);

	print_array(a1);

	print_array(a2);

	a1[1] = 1;

	print_array(a1);

	print_array(a2);

	a2[2] = 2;

	print_array(a1);

	print_array(a2);




	S s;
	T t{ s };



    return 0;
}

