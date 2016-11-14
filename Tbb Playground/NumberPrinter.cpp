#include "NumberPrinter.hpp"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

char * NumberPrinter::develop(size_t i)
{
	// HOW CAN ONE DO A LOG2(0) ?????????????
	if (i == 0)
		i = 1;

	size_t to_malloc = (size_t)((log2(i) + 3) * sizeof(char));
	char * foo = (char*) malloc(to_malloc);

	// will mess up buffer between i and std::endl
	//std::cerr << i << std::endl;
	fprintf(stderr, "%d\n", i);
/*
	char printmsg[100];
	//sprintf(printmsg, "%Iu\t%Iu\t%p\n", i, to_malloc, foo);
	fprintf(stderr, printmsg);
*/
	if (foo == NULL)
	{
		exit(-1);
	}

	sprintf(foo, "s%d", i);

	return foo;
}


void NumberPrinter::operator()(const blocked_range<size_t>& r)
{
	for (size_t i = r.begin(); i != r.end(); ++i)
		my_vect.push_back(develop(i));
}

NumberPrinter::NumberPrinter(NumberPrinter& x, split)
{}

NumberPrinter::NumberPrinter(int highbit)
{}

void NumberPrinter::join(const NumberPrinter& y)
{
	my_vect.reserve(my_vect.size() + y.my_vect.size());

	for (int i = 0; i < y.my_vect.size(); ++i)
		my_vect.push_back(y.my_vect.at(i));

	// my_vect.insert(my_vect.end(), y.my_vect.begin(), y.my_vect.end());
}