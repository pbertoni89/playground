#include "../include/xspectra_cuda_api.hpp"

#include "../include/vector_utils.h"



namespace pietra
{
	namespace cuda
	{
		int xspectra_cuda_api_foo(int a)
		{
			pietra::cuda::vector_test(a);

			return a * 2;
		}
	}
}
