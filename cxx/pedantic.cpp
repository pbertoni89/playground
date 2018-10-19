
#include <iostream>
#include <fstream>
#include <cstdint>


int foo(const int _npixels, const int _nbins)
{
	constexpr size_t BYTES_PER_COL = sizeof(uint16_t) * _npixels * _nbins;
	size_t ncols = 0;
	std::ifstream ifs("/media/xnext/DATA/data/denoising/bkg.dat", std::ifstream::binary);

	if (ifs.good())
	{
		char buff[BYTES_PER_COL];
		while (ifs.eof() == false)
		{
			ifs.read(buff, BYTES_PER_COL);
			ncols ++;
		}
		ifs.close();
	}
	else
	{
		std::cerr << " doens exist";
	}
	std::cout << ncols << " cols read " << std::endl;
}


int main()
{
	foo(4, 5);
	
}
