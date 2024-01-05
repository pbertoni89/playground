#include <array>
#include <iostream>

// What's (&) for? Removing would trigger error: creating array of references. OK
template <typename T, std::size_t N>
constexpr std::size_t arraySize(const T (&) [N]) noexcept
{
        return N;
}

void scottUsage()
{
	int b[5];
	std::array<short, arraySize(b)> c;
        std::cout << arraySize(b) << " = " << c.size() << "\n";
}

#if 1
template <typename T, std::size_t N>
void myUsage(const T (& arr) [N])
{
	const auto LEN = arraySize(arr);
	for (auto i=0; i<LEN; i++)
		std::cout << arr[i] << "\t";
}
#else
void myUsage(int * b) {};
#endif

int main()
{
	scottUsage();

        int a[7];
	myUsage(a);
}
