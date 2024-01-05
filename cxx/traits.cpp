#include <iostream>
#include <memory>
#include <type_traits>


//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
// https://accu.org/index.php/journals/442

// "is_void" failing build... std collision ?

template<typename T> struct
is_voida
{
	static const bool value = false;
};

template<> struct
is_voida< void >
{
	static const bool value = true;
};


class ObjectA
{};


class ObjectB
{
public:
	void optimised_implementation()
	{}
};


template< typename T > struct
supports_optimised_implementation
{
	static const bool value = false;
};

template<> struct
supports_optimised_implementation< ObjectB >
{
	static const bool value = true;
};


template< bool b > struct
algorithm_selector
{
	template< typename T >
	static void implementation( T& object )
	{
		std::cout << "basic implem\n";
	}
};


// add a specialization for case `optimized` = TRUE
template<> struct
algorithm_selector< true >
{
	template< typename T >
	static void implementation( T& object )
	{
		std::cout << "opt implem\n";
		object.optimised_implementation();
	}
};


// API
template< typename T >
void algorithm( T& object )
{
	algorithm_selector< supports_optimised_implementation< T >::value >::implementation(object);
}


//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
// https://en.cppreference.com/w/cpp/types/decay

template<typename T, typename U>
constexpr bool is_decay_equ = std::is_same_v<std::decay_t<T>, U>;

void demo_decay()
{
	int i;
	std::unique_ptr<decltype(i)> upi1;
	std::unique_ptr<std::decay_t<decltype(*upi1)>> upi2;
	auto upi3 = std::make_unique<std::decay_t<decltype(*upi2)>>();

	static_assert
	(
		is_decay_equ<int, int> and

		std::is_same_v<decltype(upi1), decltype(upi2)> and
		std::is_same_v<decltype(upi1), decltype(upi3)> and
		not is_decay_equ<std::unique_ptr<int>, int> and

		not is_decay_equ<int, float> and
		is_decay_equ<int&, int> and
		is_decay_equ<int&&, int> and
		is_decay_equ<const int&, int> and
		is_decay_equ<int[2], int*> and
		not is_decay_equ<int[4][2], int*> and
		not is_decay_equ<int[4][2], int**> and
		is_decay_equ<int[4][2], int(*)[2]> and
		is_decay_equ<int(int), int(*)(int)>
	);
}

//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

int main()
{
	demo_decay();

	ObjectA a;
	ObjectB b;
#if 1
	algorithm( a );
	algorithm( b );
#endif

	return 0;
}
