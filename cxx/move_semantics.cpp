// tensorflow/cc/example/example.cc

#include <iostream>
#include <vector>
#include <string>

using namespace std;


int main()
{
	vector<string> v;
	string s { "hello world" };

	cout << "1 " << s << endl;

	v.push_back(string("hello world")); // move possible, as string is an rvalue (and std::string implements move semantics)
	v.emplace_back("hello world");      // just illustrating how we do the same the less-verbose C++11 way
	v.push_back(s);                     // move can't be done, as parameter is an rvalue
	v.push_back(std::move(s));          // move possible, as we're explicitly moving the data out of s,
	// making s now be empty

	cout << "2 " << s << endl;

	for (const auto & S : v)
	{
		cout << "\t" << S << endl;
	}
}
