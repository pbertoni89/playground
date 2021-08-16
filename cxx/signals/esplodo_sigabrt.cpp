#include <iostream>


int main()
{
	std::cerr << "ADESSO ESPLODO con SIGABRT" << std::endl;
	throw std::runtime_error("sbem");
}
