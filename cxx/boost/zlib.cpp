/*
 * https://stackoverflow.com/questions/48034374/compressing-boost-binary-archive-with-zlib
 * https://stackoverflow.com/questions/4276203/how-can-i-serialize-an-stdvector-with-boostserialization
 *
 * g++ -Os -std=c++11 -Wall -pedantic -Wextra zlib.cpp -lboost_system -lboost_serialization -lboost_iostreams && ./a.out
 *
 * https://unix.stackexchange.com/questions/22834/how-to-uncompress-zlib-data-in-unix
 * $ sudo apt install qpdf
 * $ zlib-flate -uncompress < save.z
 */

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>


struct MyClass
{
	int a;
	std::string b;
	uint8_t c[8];
	std::vector<int> d;

	template <class Archive>
	void serialize(Archive & ar, unsigned)  // version
	{
		ar & a & b & c & d;
	}

	bool operator == (const MyClass & oth) const
	{
		for (size_t i=0; i<8; i++)
			if (c[i] != oth.c[i])
				return false;
		if (d.size() != oth.d.size())
			return false;
		for (size_t i=0; i<d.size(); i++)
			if (d[i] != oth.d[i])
				return false;
		return a == oth.a && b == oth.b;
	}
};


static inline std::ostream & operator << (std::ostream & os, const MyClass & obj)
{
	os << "{ a: " << obj.a << ", b: " << obj.b << ", c: ";
	for (size_t i=0; i<8; i++)
		os << obj.c[i] << ",";
	os << " d: ";
	for (size_t i=0; i<obj.d.size(); i++)
		os << obj.d[i] << ",";
	return os << " }";
}


template <class T>
void save(const T & obj, const std::string & fname)
{
	std::ofstream ofs(fname, std::ios::binary);
	{
		boost::iostreams::filtering_ostreambuf fos;

		// push the ofstream and the compressor
		fos.push(boost::iostreams::zlib_compressor(boost::iostreams::zlib::best_compression));
		fos.push(ofs);

		// start the archive on the filtering buffer
		boost::archive::binary_oarchive bo(fos);
		bo << obj;
	}
}


template <class T>
T load(const std::string & fname)
{
	std::ifstream ifs(fname, std::ios::binary);
	{
		boost::iostreams::filtering_istreambuf fis;

		// push the ifstream and the compressor
		fis.push(boost::iostreams::zlib_decompressor());
		fis.push(ifs);

		// start the archive on the filtering buffer
		boost::archive::binary_iarchive bi(fis); 
		T obj;
		bi >> obj;

		return obj;
	}
}


int main()
{
	const MyClass original{ 4, "This is a long string.", {67, 68, 69, 70, 71, 72, 73, 74}, {-1, 40} };

	const std::string FN = "/tmp/savecpp.z";
	save(original, FN);
	const auto roundtrip = load<MyClass>(FN);

	std::cout << "original:\t"  << original << "\n";
	std::cout << "roundtrip:\t" << roundtrip << "\n";
	std::cout << "equal? "	  << std::boolalpha << (original == roundtrip) << "\n";
}
