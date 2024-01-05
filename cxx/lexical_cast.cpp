#include <charconv>
#include <sstream>
#include <iostream>


template <typename E>
auto enum_class_as_integer(const E & value)
	-> typename std::underlying_type<E>::type
{
	return static_cast<typename std::underlying_type<E>::type>(value);
}



// FIXME  THIS DOES NOT WORK FOR SOME REASON 2020-12-16

template<typename N>
N lexical_cast(const std::string_view & s)
{
	auto value = N {};
	auto [ptr, error] = std::from_chars(s.data(), s.data() + s.size(), value);

	if (error != std::errc{})
	{
		std::stringstream ss; ss << "cannot convert " << s << " because " << enum_class_as_integer(error);
		throw std::runtime_error(ss.str());
	}
	return value;
}

int main ()
{

	const std::string si = "-8", sf = "3.14", su = "8u";
	// std::string_view siw{si.c_str(), si.size()};

	std::cout << si << " -> " << lexical_cast<int>(si) << std::endl;

	std::cout << sf << " -> " << lexical_cast<float>(sf) << std::endl;

	// std::cout << sf << " -> " << lexical_cast<double>(sf) << std::endl;

	std::cout << su << " -> " << lexical_cast<unsigned int>(su) << std::endl;

	return 0;
}
