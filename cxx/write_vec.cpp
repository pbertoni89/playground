// https://stackoverflow.com/questions/12372531/reading-and-writing-a-stdvector-into-a-file-correctly

#include <iterator>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>


template<typename T>
std::vector<T> parse_stream(std::istream & stream)
{
    std::vector<T> v;
    std::istream_iterator<T> input(stream);
    std::copy(input, std::istream_iterator<T>(), std::back_inserter(v));
    return v;
}


std::string path("/home/xnext/Documents/eja.bin");

const int DIM = 8;
using mine_t = uint16_t;
mine_t array[DIM] = {1, 2, 256, 4, 5, 6, 7, 65000};

const size_t BYTES_RW = DIM * sizeof(mine_t);


void fill_file()
{
	std::vector<mine_t> vecW(array, array + DIM);

	std::ofstream ofs(path, std::ios::out | std::ofstream::binary);

	auto ptrW = reinterpret_cast<char*> (vecW.data());

	ofs.write(ptrW, BYTES_RW);
}


void first_version_reads_zeros()
{
	std::ifstream ifs(path, std::ios::in | std::ifstream::binary);
	std::vector<mine_t> vecR(DIM);

	auto ptrR = reinterpret_cast<char*> (vecR.data());

	ifs.read(ptrR, BYTES_RW);
	
	for (const auto & I : vecR)
	{
		std::cout << "\n - " << I;
	}
}


void second_version()
{
	std::ifstream ifs(path, std::ios::in | std::ifstream::binary);
	if (ifs)
	{
		mine_t myVal;
		while (ifs >> myVal)
		{
			std::cout << "got " << myVal << "\n";
		}
		std::cout << "second end\n";
	}
	else
	{
		std::cerr << "no ifs!\n";
	}
}


void third_version()
{
	std::ifstream ifs(path, std::ios::in | std::ifstream::binary);
	auto v = parse_stream<mine_t>(ifs);
    for(auto & item : v)
    {
        std::cout << "3rd: " << item << "\n";
    }
    std::cout << "3rd size " << v.size() << "\n";
}


void vector_copy()
{
	std::vector<int> v1(10), v2(10, 5), v3(10, 0);
	for (int i = 0; i < v1.size(); i++)
		v1[i] = i *10;
	std::copy(v1.begin() + 3, v1.begin() + 5, v3.begin() + 6);
	
	std::cout << "\nv1";
	for (auto & i : v1)
        std::cout << " " << i;
	std::cout << "\nv2";
	for (auto & i : v2)
		std::cout << " " << i;
	std::cout << "\nv3";
	for (auto & i : v3)
        std::cout << " " << i;
}


void c_version()
{
	std::vector<mine_t> vecR(DIM);

	auto ptrR = reinterpret_cast<char*> (vecR.data());

	FILE * fp = fopen(path.c_str(), "rb");
	if (fp)
	{
		fread(ptrR, sizeof(mine_t), DIM, fp);
	}

	for (const auto & I : vecR)
	{
		std::cout << "\n - " << I;
	}
}


int main()
{
	vector_copy();

	fill_file();

	// first_version_reads_zeros();
	// second_version();
	//third_version();
	c_version();

	std::cout << std::endl;	
}
