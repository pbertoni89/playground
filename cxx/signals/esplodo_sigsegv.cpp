#include <iostream>


int main()
{
	std::cerr << "ADESSO ESPLODO con SIGSEGV" << std::endl;
	int * p = nullptr;
	std::cout << "got p " << p << std::endl;
	std::cout << "got &p " << &p << std::endl;
	std::cout << "got *p " << *p << std::endl;
}
