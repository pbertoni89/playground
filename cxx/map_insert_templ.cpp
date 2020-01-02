
#include <map>
#include <string>
#include <memory>


class A {};


template<typename T>
class C :
	public A
{
};


int main()
{
	std::map<std::string, std::unique_ptr<A>> m;
	
	std::unique_ptr<A> up = std::make_unique<C<int>>();
	
	m.insert({{"foo"}, std::move(up)});
}
