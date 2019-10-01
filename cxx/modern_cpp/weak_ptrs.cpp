#include <iostream>
#include <memory>
 
std::weak_ptr<int> wp;


void observe()
{
	std::cout << "use_count == " << wp.use_count() << ": ";

	if (auto spt = wp.lock())
	{
		// Has to be copied into a shared_ptr before usage
		std::cout << *spt << "\n";
	}
	else
	{
		std::cout << "wp is expired\n";
	}
}


int main()
{
	{
		const auto sp = std::make_shared<int>(42);

		wp = sp;

		observe();
	}

	observe();
}
