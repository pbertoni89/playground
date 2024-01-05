#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <algorithm>

using namespace std;

// -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -

class Foo
{
public:
	int m_i;
	Foo() : m_i(-1)     { std::cout << "def ctor\n"; }
	Foo(int i) : m_i(i) { std::cout << "val ctor\n"; }
	Foo(const Foo & F) : m_i(F.m_i) { std::cout << "cpy ctor\n"; }
	Foo & operator = (const Foo & F)
	{
		std::cout << "ass operator: ";
		if (this == &F)
		{
			std::cout << "equal\n";
		}
		else
		{
			std::cout << "nequal\n";
			m_i = F.m_i;
		}
		// https://stackoverflow.com/questions/1734628/copy-constructor-and-operator-overload-in-c-is-a-common-function-possible
		return *this;
	}

	friend std::ostream & operator << (std::ostream & o, const Foo & F)
	{
		o << "F addr " << &F /* << " (m_i " << &(F.m_i) << ")" */ << "\n";
		return o;
	}
};


void demo_class()
{
	Foo f1(3);
	std::cout << f1;
	auto f2 = f1;     // copy ctor
	std::cout << f2;
	Foo f4 = f1;      // copy ctor
	std::cout << f4;
	auto f3(f1);      // copy ctor
	std::cout << f3;
	f3 = f2;
	std::cout << f3;
	f1 = f1;
	std::cout << f1;

	std::cout << "\n\n\n\n";
}


// -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -

#define HW "hello world"
void demo_string()
{
	vector<string> v;
	string s {HW};

	cout << "1 " << s << endl;

	v.push_back(string(HW));        // move possible, as string is an rvalue (and std::string implements move semantics)
	v.push_back(HW);                // TODO how does it solves?
	v.emplace_back(HW);             // just illustrating how we do the same the less-verbose C++11 way
	v.push_back(s);                 // move can't be done, as parameter is an rvalue
	v.push_back(std::move(s));      // move possible, as we're explicitly moving the data out of s, making s now be empty

	cout << "2 " << s << endl;

	for (const auto & S : v)
		cout << "\t" << S << endl;
}


// -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -


class Shape {};
class Triangle : public Shape {};


template<typename T>
class unique_ptr
{
	T* ptr;

public:

	T* operator->() const { return ptr; }

	T& operator*() const { return *ptr; }

	explicit unique_ptr(T* p = nullptr) { ptr = p; }

	~unique_ptr() { delete ptr; }

	unique_ptr(unique_ptr && source)   // note the rvalue reference
	{
		ptr = source.ptr;
		source.ptr = nullptr;
	}

#if 1
	/** Move Assignment Operator  */
	unique_ptr & operator = (unique_ptr && source)   // note the rvalue reference
	{
		std::cout << "MAO 1\n";
		if (this != &source)    // beware of self-assignment
		{
			delete ptr;         // release the old resource
			ptr = source.ptr;   // acquire the new resource
			source.ptr = nullptr;
		}
		return *this;
	}
#else
	/** Move Constructor          */
	unique_ptr & operator = (unique_ptr source)     // note the missing reference
	{
		std::cout << "MAO 2\n";
		std::swap(this->ptr, source.ptr);
		return *this;
	}
#endif
};


unique_ptr<Triangle> factory () { return unique_ptr<Triangle>(new Triangle); }


void demo_ptr()
{
	unique_ptr<Triangle> t1(new Triangle);
	unique_ptr<Triangle> t2(new Triangle);
	// unique_ptr<Shape> b(a);  // won't compile: a is an LVALUE
	t1 = factory();
}


// -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -

#define EXPLICIT_COPY_MOVE  // needed

class Container
{
public:
	int m_i;
	std::vector<int> m_vi;

	explicit Container(size_t sz) :
		m_i(-1),
		m_vi(sz, 7)
	{ std::cout << "Container(" << sz << ")\n"; };

#ifdef EXPLICIT_COPY_MOVE
	Container(const Container & oth) : m_vi(oth.m_vi)
	{ std::cout << "C_Container(oth) \n"; }

	Container & operator = (const Container & oth)
	{ m_vi = oth.m_vi;  std::cout << "C_Op(oth) \n"; return *this; }

	Container(Container && oth) : m_vi(std::move(oth.m_vi))
	{ std::cout << "M_Container(oth) \n"; }

	Container & operator = (Container && oth)
	{ m_vi = std::move(oth.m_vi);  std::cout << "M_Op(oth) \n"; return *this; }
#endif

	friend std::ostream & operator << (std::ostream & o, const Container & c)
	{
		o << "{int " << c.m_i << ", v[";
		for (auto i : c.m_vi)
			o << " " << i;
		o << "]}"; return o;
	}
};

void demo_container()
{
	std::cout << "-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  \n\n";
	Container c1(10);
	c1.m_i = 1;
	auto c2(c1);
	c2.m_i = 2;
	std::cout << "c1: " << c1 << "\n";
	std::cout << "c2: " << c2 << "\n\n";

	auto c3(std::move(c2));
	std::cout << "c1: " << c1 << "\n";
	std::cout << "c2: " << c2 << "\n";    // empty
	std::cout << "c3: " << c3 << "\n\n";  // c3: {int -1229489104, front 7} if MOVE_EXPLICIT is defined and we do not set rules on m_i

	c3 = c1;
	std::cout << "c1: " << c1 << "\n";
	std::cout << "c2: " << c2 << "\n";    // empty
	std::cout << "c3: " << c3 << "\n\n";  // c3: {int -1229489104, front 7} if MOVE_EXPLICIT is defined and we do not set rules on m_i

	c3 = std::move(c1);
	std::cout << "c1: " << c1 << "\n";    // empty
	std::cout << "c2: " << c2 << "\n";    // empty
	std::cout << "c3: " << c3 << "\n\n";  // c3: {int -1229489104, front 7} if MOVE_EXPLICIT is defined and we do not set rules on m_i
}

// -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -  / -

int main()
{
	demo_class();
	demo_string();
	demo_ptr();
	demo_container();
}
