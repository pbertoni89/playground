#include "../../libxspectra-cuda/include/xspectra_cuda_api.hpp"

#include <iostream>


int main()
{
	const int TEST = 500000;
	const int RET = pietra::cuda::xspectra_cuda_api_foo(TEST);

	std::cout <<
#ifdef DEBUG
			"DEBUG"
#else
			"RELEASE"
#endif
			" - xspectra_cuda_api_foo(" << TEST << ") = " << RET << std::endl;

	const int EXITVAL = (TEST * 2 != RET);
	std::cout << "Exiting with " << EXITVAL;
	return EXITVAL;
}
