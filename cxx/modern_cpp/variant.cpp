// https://www.quora.com/What-is-something-you-re-sick-of-seeing-in-C-code-1

#include <variant>
#include <iostream>
#include <vector>
#include <cassert>

#pragma message "C++17 standard is required"


template <typename Communicator>
class animal : public Communicator
{
};

class dog_communicator
{
public:
	void communicate() const { std::cout << "B"; }
};

class cat_communicator
{
	char * p = new char[1000];
public:
	void communicate() const { std::cout << "M"; }
};


using dog = animal<dog_communicator>;
using cat = animal<cat_communicator>;
using an_animal = std::variant<dog, cat>;


void do_something(const std::vector<an_animal> & V_A)
{
	for (auto & e : V_A)
	{
		std::visit ([] (auto & a) { a.communicate(); }, e);
	}
}


void demo1()
{
	std::vector<an_animal> animals;
	for (size_t i = 0; i < 1'000; i++)
	{
		// ternary operator is forbidden: "different types"
		if (i % 2 == 0)
			animals.push_back(dog());
		else
			animals.push_back(cat{});
	}
	
	do_something(animals);
}


void demo2()
{
	std::variant<int, float, char> v, w;
	v = 12; // v contains int
	int i = std::get<int>(v);
	w = std::get<int>(v);
	w = std::get<0>(v); // same effect as the previous line
	w = v; // same effect as the previous line

	//  std::get<double>(v); // error: no double in [int, float]
	// std::get<3>(v);      // error: valid index values are 0, 1, 2

	try {
		std::get<float>(w); // w contains int, not float: will throw
	}
	catch (const std::bad_variant_access &) {}

	using namespace std::literals;

	std::variant<std::string> x("abc");
	// converting constructors work when unambiguous
	x = "def"; // converting assignment also works when unambiguous

	std::variant<std::string, void const*> y("abc");
	// casts to void const * when passed a char const *
	assert(std::holds_alternative<void const*>(y)); // succeeds
	y = "xyz"s;  // that's what std::literals is for
	assert(std::holds_alternative<std::string>(y)); // succeeds
}


int main()
{
	demo1();
	demo2();
}
