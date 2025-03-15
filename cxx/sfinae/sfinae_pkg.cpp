#include <iostream>
#include <vector>
#include <cstdint>

using my_t = uint64_t;

// Own implementation of 3D-vector. size() == m_X * m_Y * m_Z
class Package
{
	// 3D-strides
	const size_t m_X = 10, m_Y = 2, m_Z = 4;

public:

	std::vector<my_t> m_v;

	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<typename std::iterator_traits<I>::value_type>::iterator>::value, std::vector<my_t>::iterator>::type
	get_it(I i, size_t x, size_t y, size_t z)
	{
		return std::next(i, x * m_X * m_Y + y * m_Z + z);
	}

	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<typename std::iterator_traits<I>::value_type>::const_iterator>::value, std::vector<my_t>::const_iterator>::type 
	get_it(I i, size_t x, size_t y, size_t z) const
	{
		return std::next(i, x * m_X * m_Y + y * m_Z + z);
	}

	std::vector<my_t>::iterator
	get_m(size_t x, size_t y, size_t z)
	{
		return get_it(m_v.begin(), x, y, z);
	}

	std::vector<my_t>::const_iterator
	get_c(size_t x, size_t y, size_t z) const
	{
		return get_it(m_v.cbegin(), x, y, z);
	}

	my_t & operator () (size_t x, size_t y, size_t z) { std::cout << "(m)\n"; return *(m_v.begin() + x * m_X * m_Y + y * m_Z + z); }
	my_t operator () (size_t x, size_t y, size_t z) const { std::cout << "(c)\n"; return *(m_v.begin() + x * m_X * m_Y + y * m_Z + z); }

	Package() : m_v(m_X * m_Y * m_Z, 1)
	{}
};


void f_const(const Package & pkg)
{
	auto it = pkg.get_c(0, 1, 2);
	auto val = pkg(0, 1, 2);
	std::cout << "const: " << *it << ", " << val << "\n";
}

void f_modif(Package & pkg)
{
	auto it = pkg.get_m(0, 1, 2);	// no need (even wrong) to mantain a reference to an iterator
	*it += 10;
	auto val = pkg(0, 1, 2); 		// without &, val resolves to my_t, even if (m) is called
	val += 90;
	auto & ref = pkg(0, 1, 2);
	ref += 800;
	std::cout << "modif_1: " << *it << ", " << val << ", " << ref << "\n";
	pkg(0, 1, 2) = 70;
	std::cout << "modif_2: " << *it << ", " << val << ", " << ref << "\n";
}

int main(int argc, char**argv)
{
	Package p;
	f_const(p);
	std::cout << "\n";
	f_modif(p);
}
