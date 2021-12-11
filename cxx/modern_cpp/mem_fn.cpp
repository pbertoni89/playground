#include <functional>
#include <iostream>
 
class Foo {
	public:
	void display_greeting() {
		std::cout << "Hello, world.\n";
	}
	void display_number(int i) {
		std::cout << "number: " << i << '\n';
	}
	void display_number() {
		std::cout << "default number: " << 56 << '\n';
	}
	int data = 7;
};


int main() {
	Foo f;

	auto greet = std::mem_fn(&Foo::display_greeting);
	greet(f);

	auto print_num = std::mem_fn<void(int)>(&Foo::display_number);
	print_num(f, 42);

	auto access_data = std::mem_fn(&Foo::data);
	std::cout << "data: " << access_data(f) << '\n';
}
