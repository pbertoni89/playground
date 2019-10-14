
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




class C             { public: virtual void bark() { std::cout << "c\n"; } };
class Z  : public C { public: virtual void bark() override { std::cout << "z\n"; } };
class PR : public Z { public: virtual void bark() override { std::cout << "pr\n"; } };
class PC : public Z { public: virtual void bark() override { std::cout << "pc\n"; } };
class TL : public C { public: virtual void bark() override { std::cout << "tl\n"; } };

//void foo(std::shared_ptr<C> a)          // last resort
//{ std::cout << "foo C "; a->bark(); }
void foo(std::shared_ptr<Z> a)
{ std::cout << "foo Z "; a->bark(); }
void foo(std::shared_ptr<PR> a)
{ std::cout << "foo PR "; a->bark(); }
//void foo(std::shared_ptr<PC> a)         // wow, SFINAE solves to Z
//{ std::cout << "foo PC "; a->bark(); }
void foo(std::shared_ptr<TL> a)
{ std::cout << "foo TL "; a->bark(); }

int demo_overload()
{
    C c;
    Z z;
    PR pr;
    PC pc;
    TL tl;
    std::cout << std::is_same<C, Z>::value << "\n";   // false
    std::cout << std::is_same<PR, PC>::value << "\n";   // false
    std::cout << std::is_same<decltype(pr), PR>::value << "\n";   // true
    std::cout << std::is_same<decltype(pr), Z>::value << "\n";   // false
    //foo(std::make_shared<C>());
    foo(std::make_shared<Z>());
    foo(std::make_shared<PR>()); foo(std::make_shared<PC>());
    foo(std::make_shared<TL>());
}


int main (int argc, char ** argv)
{
	demo_overload();

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
