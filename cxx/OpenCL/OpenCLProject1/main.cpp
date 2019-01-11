#include "SaxpyOclApplication.hpp"
#include "ImageOclApplication.hpp"
#include "intel_1.h"

#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	int choice;
	std::cout << "0 - intel_1" << std::endl;
	std::cout << "1 - saxpy" << std::endl;
	std::cout << "2 - image" << std::endl;
	std::cout << std::endl << "Select and press enter" << std::endl;
	std::cin >> choice;

	OclApplication * oa;

	try
	{
		switch (choice)
		{
		case 0:
			do_intel_1();
			break;
		case 1:
			oa = new SaxpyOclApplication();
			break;
		case 2:
			oa = new ImageOclApplication();
			break;
		default:
			exit(-1);
		}

		oa->run();

		std::cout << std::endl << std::endl << "RUN OK" << std::endl;
		delete oa;
	}
	catch (std::runtime_error re)
	{
		std::cerr << std::endl << std::endl << "RUNTIME ERROR: " << re.what() << std::endl;
	}
	
	std::cerr << "Remember to add \"-cl-opt-disable\" to kernel parameters!" << std::endl;
	system("pause");
}
