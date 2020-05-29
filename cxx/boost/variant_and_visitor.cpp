/*
 * https://www.boost.org/doc/libs/1_64_0/doc/html/variant.html
 */

#include "boost/variant.hpp"
#include <iostream>
#include <cmath>


using t_c = char; // tried also with unsigned, same

class My
{
public:
	const t_c m_c;

	explicit My(t_c c) : m_c(c) { std::cout << "MyCtor_" << m_c << "\n"; }

	My(My & ot)  : m_c(ot.m_c)  { std::cout << "MyCopy_" << m_c << "\n"; }
	My(My && ot) : m_c(ot.m_c)  { std::cout << "MyMove_" << m_c << "\n"; }

	~ My()                      { std::cout << "MyDtor_" << m_c << "\n"; }

	friend std::ostream & operator << (std::ostream & o, const My & m)
	{
		o << "MyOStr" << m.m_c;
		return o;
	}
};


template <typename T = int>
class MyVisitor :
	public boost::static_visitor<T>
{
	const int m_key;

public:
	MyVisitor(int key) : m_key(key) {};

	T operator() (int i) const
	{
		std::cout << "key " << m_key << " an int: ";
		return sqrt(i);
	}

	T operator() (const std::string & str) const
	{
		std::cout << "key " << m_key << " a string: ";
		return str.length();
	}

	T operator() (double d) const
	{
		std::cout << "key " << m_key << " a double: ";
		return (T) d;
	}

	T operator() (const My & my) const
	{
		std::cout << "key " << m_key << " a My: ";
		return my.m_c;
	}
};

using t_v = boost::variant<int, std::string, double, My>;
using t_vref = boost::variant<int, std::string, double, My &>;


void foo(t_v & var)
{
	std::cout << var << " -v-> " << boost::apply_visitor(MyVisitor<>(88), var) << '\n';
}

void foo(t_vref & var)
{
	std::cout << var << " -r-> " << boost::apply_visitor(MyVisitor<>(99), var) << '\n';
}


int main()
{
	{
		// My m('A');
		// std::cout << m << '\n';           // My__A

		t_v vs("hello world"), vi(25), vd(3.14), vmm(My('B')), vmi{'C'};
		foo(vs);
		foo(vi);
		foo(vd);
		foo(vmm);   // 65 -> an int: 8
		foo(vmi);   // 65 -> an int: 8
	}
	std::cout << "\n\n\n";
	{
		t_vref vr1{(t_c) 'D'};
		// t_vref vr2(My('E'));  // CRASH 
		My m('E');
		t_vref vr2(m);

		foo(vr1);   // 65 -> an int: 8
		foo(vr2);   // 65 -> an int: 8
	}
}
