#include <memory>
#include <iostream>
#include <variant>
#include <string>
#include <cassert>


int variant_demo()
{
	std::variant<int, float> v, w;
	v = 12; // v contains int
	int i = std::get<int>(v);
	w = std::get<int>(v);
	w = std::get<0>(v); // same effect as the previous line
	w = v; // same effect as the previous line

	//  std::get<double>(v); // error: no double in [int, float]
	//  std::get<3>(v);      // error: valid index values are 0 and 1

	try
	{
		std::get<float>(w); // w contains int, not float: will throw
	}
	catch (const std::bad_variant_access&) {}

	using namespace std::literals;

	std::variant<std::string> x("abc");
	// converting constructors work when unambiguous
	x = "def"; // converting assignment also works when unambiguous

	std::variant<std::string, void const*> y("abc");
	// casts to void const * when passed a char const *
	assert(std::holds_alternative<void const*>(y)); // succeeds
	y = "xyz"s;
	assert(std::holds_alternative<std::string>(y)); // succeeds
}



class A {};
class PC : public A {};
class PR : public A {};

class M
{

public:

	virtual void accept(std::shared_ptr<PC> sp)  { std::cout << "PC\n"; }

	// How to ensure the overload call to right derived type ?
	virtual void accept(std::shared_ptr<A> sp)   { std::cout << "A\n"; }

	virtual void accept(std::shared_ptr<PR> sp)  { std::cout << "PR\n"; }
};


// void accept(std::shared_ptr<A>) { std::cout << "VA "; }
// void accept(std::variant<std::shared_ptr<A>, std::shared_ptr<PC>> vapc) { std::cout << "VAPC "; }
void accept(std::variant<std::shared_ptr<A>, std::shared_ptr<PR>> vapr) { std::cout << "VAPR "; }


int main(int argc, char**argv)
{
	variant_demo();

	auto a = std::make_shared<A>();
	auto pc = std::make_shared<PC>();
	auto pr = std::make_shared<PR>();
	std::shared_ptr<A> apr = std::make_shared<PR>();    // !!!

	M m;
	m.accept(a);
	m.accept(pc);
	m.accept(pr);
	m.accept(apr);                                        // !!!

	// accept(a);
	accept(pc);
	accept(pr);
	accept(apr);
}
