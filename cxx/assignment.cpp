#include <cstdlib>
#include <vector>
#include <iostream>
#include <iomanip>


#define DIM 12
#define SUBDIM 4

/*
 * 
	int myints[]={10,20,30,40,50,60,70};
	std::vector<int> myvector (7);

	std::copy ( myints, myints+7, myvector.begin() );
 */


int main()
{
	int array[DIM];

	for (int i=0; i<DIM; i++)
	{
		array[i] = rand() % 1000;
		std::cout << std::setw(4) << array[i] << "\t";
	}
	std::cout << std::endl;

	std::vector<int> va(DIM);
	va.assign(array, array + DIM);
	
	std::vector<int> vc(array, array + DIM);

	for (const auto & ia : va)
	{
		std::cout << std::setw(4) << ia << "\t";
	}
	std::cout << std::endl;
	
	for (const auto & ic : vc)
	{
		std::cout << std::setw(4) << ic << "\t";
	}
	std::cout << std::endl;
	
	const int NSUB = (DIM  /SUBDIM);  // 3

	for (int id=0; id<NSUB; id++)
	{
		std::vector<int> vd(SUBDIM);
		const int POS = (id*SUBDIM);

		for (int idb=0; idb<POS; idb++)
		{
			std::cout << "    \t";
		}
		for (int idi=0; idi<SUBDIM; idi++)
		{
			vd[idi] = rand() % 20;
			std::cout << std::setw(4) << vd[idi] << "\t";
		}
		for (int ida=id+1; ida<NSUB; ida++)
		{
			std::cout << "    \t";
		}
		std::cout << std::endl;
		
		std::copy(vd.begin(), vd.end(), vc.begin() + POS);

		// print after copy
		for (const auto & ic : vc)
		{
			std::cout << std::setw(4) << ic << "\t";
		}
		std::cout << std::endl;	
	}
	
}
