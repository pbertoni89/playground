#include <set>
#include <vector>
#include <iostream>
#include <sstream>


// https://stackoverflow.com/questions/44655222/c-why-template-cannot-be-used-to-deduce-both-container-and-element-type
// template<typename C, typename E>
// std::ostream & operator << (std::ostream & o, const C<E> & cont)

template<template <typename...> class C, typename... E>
std::ostream & operator << (std::ostream & o, const C<E...> & cont)
{
	o << "C{";
	for (auto i : cont) o << "\t" << i;
	o << "}";
	return o;
}


void change(std::set<int> & s)
{
	for (auto i : s)  // elements are copied: OK
	 	i ++;
	//for (auto & i : s)  // elements are referenced: won't work because is a SET !
	// 	i ++;
	//for (auto it = s.begin(); it != s.end(); it++)   // idem
	//	(*it) ++;
}


void change(std::vector<int> & v)
{
	for (auto i : v)
	 	i ++;
	for (auto & i : v)
	 	i ++;
	for (auto it = v.begin(); it != v.end(); it++)
		(*it) ++;
}


void demo_blocks()
{
	std::set<int> m_ap_runtime = {2, 3, 4, 5,     60,      62, 63,      300,      301, 302, 303,     400,      401};
	std::set<std::set<int>> m_ap_blocks = {{2}, {3, 4}};
	std::stringstream ss;
	std::cout << "APC Before:" << m_ap_runtime << "\n\n";

	//m_ap_runtime.insert(4);
	//m_ap_runtime.erase(2);

	// Re-set blocks
	m_ap_blocks.clear();
	auto it = m_ap_runtime.cbegin();
	while (it != m_ap_runtime.cend())
	{
		std::set<int> block{*it};
		while (*(++it) - *block.crbegin() == 1 and it != m_ap_runtime.cend())
		{
			block.insert(*it);
		}
		m_ap_blocks.insert(block);
	}

	std::cout << "APC After:" << m_ap_runtime << "\n\n" << m_ap_blocks << "\n";
}


int main (int argc, char**argv)
{

	std::set<int> s0;
	std::set<int> s1{2, 4, 65, 7};

	std::cout << s0 << "\n";
	std::cout << s1 << "\n";
	change(s1);
	std::cout << s1 << "\n";

	//std::cout << s1.begin() << "\n";
	std::cout << "beg  " << *s1.begin() << "\n";
	std::cout << "end  " << *s1.end() << "\n";
	std::cout << "rbeg " << *s1.rbegin() << "\n";
	std::cout << "rend " << *s1.rend() << "\n";
	//std::cout << s0.begin() << "\n";
	std::cout << *s0.begin() << "\n";
	std::cout << *s0.end() << "\n";

	std::set<std::set<int>> ss0 = {{1, 2}, {0, 10}, {60}, {-1, 1, 1, 11}, {1, 1, 1}};
	std::cout << ss0;

	std::cout << "\n-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  \n";
	std::vector<int> v0 {9, 4, -1};
	std::cout << v0 << "\n";
	change(v0);
	std::cout << v0 << "\n";
	
	std::cout << "\n\n";

	demo_blocks();
	return 0;
}
