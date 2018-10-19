
#include <iostream>


#if 0

/**
 * @return int iff a, b are ints; else return double
 */
template <type1,type2>
typename determine(type1 a, type2 b)
{
    if (typeid(type1) == typeid(int) and typeid(type2) == typeid(int))
        return int;
    else
		return double;
}
#endif


/**
 * Cover all 4 combination
 */

template <typename T1, typename T2> struct TypeSelector
{
   using type = double;
};

template <typename T1> struct TypeSelector<T1, int>
{
   using type = int;
};

template <typename T2> struct TypeSelector<int, T2>
{
   using type = int;
};

template <> struct TypeSelector<int, int>
{
   using type = int;
};



template <> struct Farlok
{
	using type = char;
};



int main()
{
#if 0
    int a = 3;
    double b = 2;
    std::cout << (static_cast<determine(a, b)>(a) / static_cast<determine(a, b)>(b)) << std::endl;
#endif

	int a = 3, b = 2;
	using type1 = TypeSelector<decltype(a), decltype(b)>::type;
	std::cout << (static_cast<type1>(a) / static_cast<type1>(b)) << std::endl;

	float c = 4.5f;
	using type2 = TypeSelector<decltype(a), decltype(c)>::type;
	std::cout << (static_cast<type2>(a) / static_cast<type2>(c)) << std::endl;

	using type3 = TypeSelector<decltype(c), decltype(a)>::type;
	std::cout << (static_cast<type3>(c) / static_cast<type3>(a)) << std::endl;

	using f = Farlok<>::type;
	//std::cout << "sizeof(?) = " << sizeof(f) << std::endl;
}
 
