#include <iostream>
#include <cstdlib>
#include <unistd.h>

//#define PROMPT

int main()
{
	int nBytes = 50000;
#ifdef PROMPT
	std::cout << "How much bytes? ";
	std::cin >> nBytes;
#endif
	std::cout << std::endl << "mallocating " << nBytes << std::endl;

	int * array = (int*) malloc(nBytes * sizeof(int));

#ifdef PROMPT
	std::cout << "End? ";
	std::cin >> nBytes;
	std::cout << std::endl;
#else
	const size_t SLEEP_US = 2*1000*1000;
	std::cout << "sleeping " << SLEEP_US << std::endl;
	usleep(2*1000*1000);
#endif

	std::cout << "freeing " << nBytes << std::endl;
	free(array);

	return 0;	
}
