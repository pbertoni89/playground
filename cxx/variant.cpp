// https://www.quora.com/What-is-something-you-re-sick-of-seeing-in-C-code-1

#include <variant>
#include <iostream>
#include <vector>
#include <cassert>

#include <type_traits>  // demo4
#include <iomanip>  // demo4

// #pragma message "C++17 standard is required"


/*
 * ----------------------------------------------------------------------
 * VISIT: ANIMALS
 */

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


/*
 * ----------------------------------------------------------------------
 * BAD ACCESS + HOLD ALTERNATIVE
 */

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


void demo3()
{
	using var_t = std::variant<int, double, char>;
	var_t v(-1), w(2.), z('c');
	std::vector<var_t> vv{-1, 2.f, 'c'};
}


/*
 * ----------------------------------------------------------------------
 * VISIT
 * https://en.cppreference.com/w/cpp/utility/variant/visit
 */


// helper constant for the visitor #3
template<class> inline constexpr bool always_false_v = false;
 
// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


void demo4()
{
	using var_t = std::variant<int, long, double, std::string>;

	std::vector<var_t> vec = {(int)3'000'000'000, 3'000'000'000l, 1.6, "hello"};

	for(auto & v: vec)
	{

		// 1. void visitor, only called for side-effects (here, for I/O)
		std::visit([] (auto && arg) {std::cout << arg;}, v);

		// 2. value-returning visitor, demonstrates the idiom of returning another variant
		var_t w = std::visit([] (auto && arg) -> var_t {return arg + arg;}, v);

		// 3. type-matching visitor: a lambda that handles each type differently
		std::cout << "\t\t\t(*2) =   ";
		std::visit([] (auto && arg)
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, int>)
				std::cout << "[int] " << arg << '\n';
			else if constexpr (std::is_same_v<T, long>)
				std::cout << "[long] " << arg << '\n';
			else if constexpr (std::is_same_v<T, double>)
				std::cout << "[double] " << arg << '\n';
			else if constexpr (std::is_same_v<T, std::string>)
				std::cout << "[std::string] " << std::quoted(arg) << '\n';
			else 
				static_assert(always_false_v<T>, "non-exhaustive visitor!");
		}, w);
	}

	for (auto & v: vec)
	{
		// 4. another type-matching visitor: a class with 3 overloaded operator()'s
		// Note: The `(auto arg)` template operator() will bind to `int` and `long`
		//       in this case, but in its absence the `(double arg)` operator()
		//       *will also* bind to `int` and `long` because both are implicitly
		//       convertible to double. When using this form, care has to be taken
		//       that implicit conversions are handled correctly.
		std::visit(overloaded
		{
			[] (auto arg) { std::cout << arg << ' '; },
			[] (double arg) { std::cout << std::fixed << arg << ' '; },
			[] (const std::string& arg) { std::cout << std::quoted(arg) << ' '; }
		}, v);
	}
}


/*
 * ----------------------------------------------------------------------
 * MAIN
 */

int main()
{
	// demo1(); std::cout << '\n';
	demo2(); std::cout << '\n';
	demo3(); std::cout << '\n';
	demo4(); std::cout << '\n';
}
