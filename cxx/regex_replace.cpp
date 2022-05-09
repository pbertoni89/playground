// regex_replace example
#include <iostream>
#include <string>
#include <regex>
#include <iterator>



void demo()
{
	const std::string s ("there is a subsequence in the string\n");
	const std::regex e ("\\b(sub)([^ ]*)");   // matches words beginning by "sub"

	// using string/c-string (3) version
	std::cout << std::regex_replace (s,e,"sub-$2");

	// using range/c-string (6) version
	std::string result;
	std::regex_replace (std::back_inserter(result), s.begin(), s.end(), e, "$2");
	std::cout << result;

	// with flags
	std::cout << std::regex_replace (s,e,"$1 and $2",std::regex_constants::format_no_copy) << std::endl;
}


/**
 * obtain `ipu_belt_0_cam_0_image_data` from `ipu_image_data` by adding `belt_0_cam_0_`
 */
void ours()
{
	const std::string basename("ipu_image_data");
	const std::string token = "ipu_",
		element = "belt_0_cam_0_";
	const std::string r = token + std::string{"$2"} + element;
	std::cout << std::regex_replace(basename, std::regex(token), r) << std::endl;
}


void foo(int && i)
{
	std::cout << "int foo with " << i << '\n';
}


template <typename T>
void foo(T && t)
{
	std::cout << "t   foo with " << t << '\n';
}




int main ()
{    
	demo();
	ours();

	foo('c');
	foo(1);

    return 0;
}
