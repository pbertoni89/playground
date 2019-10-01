/*
 * https://accu.org/index.php/journals/442
 */

#include <iostream>
using namespace std;


//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

// "is_void" failing build... std collision ?

template< typename T > struct
is_voida
{
	static const bool value = false;
};

template<> struct
is_voida< void >
{
	static const bool value = true;
};



//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

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
		cout << "basic implem\n";
	}
};


// add a specialization for case `optimized` = TRUE
template<> struct
algorithm_selector< true >
{
	template< typename T >
	static void implementation( T& object )
	{
		cout << "opt implem\n";
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

// MAIN
int main()
{
	ObjectA a;
	ObjectB b;
#if 1
	algorithm( a );
	algorithm( b );
#endif

	return 0;
}
