#include <fstream>
#include <cstdlib>

template<class T>
T read_urandom()
{
	union {
		T value;
		char cs[sizeof(T)];
	} u;

	std::ifstream rfin("/dev/urandom");
	rfin.read(u.cs, sizeof(u.cs));
	rfin.close();

	return u.value;
}

#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	cout << read_urandom<short>() << endl;
	cout << read_urandom<int>() << endl;
	cout << read_urandom<long>() << endl;
	cout << (unsigned int)read_urandom<unsigned char>() << endl;
}

