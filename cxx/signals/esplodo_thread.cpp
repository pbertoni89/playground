#include <iostream>
#include <thread>


void fun()
{
	std::cerr << "ADESSO ESPLODO con THREAD" << std::endl;
	throw std::runtime_error("sbem");
}

int main()
{
	std::cerr << "esplodero" << std::endl;
	std::thread t(fun);
	std::cerr << "vedrai" << std::endl;
	t.join();
	std::cerr << "boh" << std::endl;
}
