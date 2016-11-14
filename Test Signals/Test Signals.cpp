// THIS AVOID THE USAGE OF -D_SCL_SECURE_NO_WARNINGS.... NO, THEY DON'T WORK
// http://stackoverflow.com/questions/14327399/visual-studio-2012-c-compile-error-with-boost-signal2
//#pragma warning(push, 0)
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
//#pragma warning(pop)

#include <iostream>

#include "TextView.hpp"

void print_args(float x, float y)
{
	std::cout << "The arguments are " << x << " and " << y << std::endl;
}

void print_sum(float x, float y)
{
	std::cout << "The sum is " << x + y << std::endl;
}

void print_product(float x, float y)
{
	std::cout << "The product is " << x * y << std::endl;
}

void print_difference(float x, float y)
{
	std::cout << "The difference is " << x - y << std::endl;
}

void print_quotient(float x, float y)
{
	std::cout << "The quotient is " << x / y << std::endl;
}

int main(int argc, char** argv)
{
	boost::signals2::signal<void(float, float)> sig;

	sig.connect(&print_args);
	sig.connect(&print_sum);
	sig.connect(&print_product);
	sig.connect(&print_difference);
	sig.connect(&print_quotient);

	sig(5.0, 3.0);

	std::cout << std::endl << std::endl << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * " << std::endl << std::endl;

	Document doc;
	TextView textview(doc);
	doc.append("Negri");
	doc.append(" e il bus del cul");
}
