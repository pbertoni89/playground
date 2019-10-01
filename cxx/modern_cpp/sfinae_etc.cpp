
#include <iostream>
#include <vector>
#include <type_traits>


class Foo  // just for testing const cv-qualifier
{
	std::vector<int> m = {2, 22, 222};

public:
#if 0
	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<typename std::iterator_traits<I>::value_type>::iterator>::value, void>::type
	catchIterator(size_t i_col, size_t i_pix, size_t i_bin)
	{
		std::cout << "f0\n";
	}

	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<typename std::iterator_traits<I>::value_type>::const_iterator>::value, void>::type
	catchIterator(size_t i_col, size_t i_pix, size_t i_bin) const
	{
		std::cout << "f1\n";
	}
#else
	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<typename std::iterator_traits<I>::value_type>::iterator>::value, std::vector<int>::iterator>::type
	catchIterator(size_t i_col, size_t i_pix, size_t i_bin)
	{
		std::cout << "m\n";
		return m.begin();
	}

	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<typename std::iterator_traits<I>::value_type>::const_iterator>::value, std::vector<int>::const_iterator>::type
	catchIterator(size_t i_col, size_t i_pix, size_t i_bin) const
	{
		std::cout << "mc\n";
		return m.cbegin();
	}
#endif
	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<int>::iterator>::value, void>::type
	catchIterator(I i)
	{
		std::cout << "f2\n";
	}

	template <class I>
	typename std::enable_if<std::is_same<I, typename std::vector<int>::const_iterator>::value, void>::type
	catchIterator(I i) const
	{
		std::cout << "f3\n";
	}
};


int main (int argc, char ** argv)
{
	std::vector<int> v = {1, 11, 111};
	Foo f;
	f.catchIterator(v.begin());							// f2
	f.catchIterator(v.cbegin());						// f3
	//f.catchIterator(v.crbegin());  deduction fails!
	//f.catchIterator(v.crend());    deduction fails!
	f.catchIterator(v.end());							// f2
	f.catchIterator(v.cend());							// f3
	f.catchIterator<decltype(v.begin())>(8, 8, 8);		// #if 0 f0 #else m
	f.catchIterator<decltype(v.cbegin())>(8, 8, 8);		// #if 0 f1 #else mc
}
