#include <iostream>

int main()
{
	int * pi1 = new int;
	int * pi2 = nullptr;
	std::cout << "pi1 " << pi1 << std::endl;
	std::cout << " i1 " << *pi1 << std::endl;
	std::cout << "pi2 " << pi2 << std::endl;
	std::cout << " i2 " << *pi2 << std::endl;
}

