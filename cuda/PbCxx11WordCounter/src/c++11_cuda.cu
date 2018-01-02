/* Copyright (c) 1993-2015, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <thrust/device_ptr.h>
#include <thrust/count.h>
#include <thrust/execution_policy.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <unistd.h>


#include <helper_cuda.h>


/////////////////////////////////////////////////////////////////
// Some utility code to define grid_stride_range
// Normally this would be in a header but it's here
// for didactic purposes. Uses 
#include "range.hpp"

using namespace util::lang;



#include <sys/stat.h>

off_t fsize(const char * FILENAME)
{
	struct stat st;

	if (stat(FILENAME, &st) == 0)
	{
		return st.st_size;
	}
	else
	{
		return -1;
	}
}


/**
 * type alias to simplify typing...
 */
template<typename T>
using step_range = typename range_proxy<T>::step_range_proxy;


/**
 *
 */
template <typename T>
__device__
step_range<T> grid_stride_range(T begin, T end)
{
	begin += blockDim.x * blockIdx.x + threadIdx.x;
	return range(begin, end).step(gridDim.x * blockDim.x);
}


/**
 *
 */
template <typename T, typename Predicate>
__device__ 
void ours_count_if(int * count, T * data, int n, Predicate p)
{ 
	for (const auto i : grid_stride_range(0, n))
	{
		if (p(data[i]))
		{
			atomicAdd(count, 1);
		}
	}
}


/**
 * Use count_if with a lambda function that searches for x, y, z or w
 * Note the use of range-based for loop and initializer_list inside the functor
 * We use auto so we don't have to know the type of the functor or array
 */
__global__
void d_xyzw_frequency_ours(int * count, char * text, int n)
{
	const char letters[] { 'x','y','z','w'};

	ours_count_if(count, text, n,
			[&](char c)
			{
				for (const auto x : letters)
				{
					if (c == x)
					{
						return true;
					}
				}
				return false;
			});
}


/**
 *
 */
__global__
void d_xyzw_frequency_thrust(int *count, char *text, int n)
{
	const char letters[] = { 'x','y','z','w'};

	*count = thrust::count_if(thrust::device, text, text+n,
			[=](char c)
			{
				for (const auto x : letters)
				{
					if (c == x)
					{
						return true;
					}
				}
				return false;
			});
}


/**
 * a bug in Thrust 1.8 causes warnings when this is uncommented
 * so commented out by default -- fixed in Thrust master branch
 */
#define ALLOW_THRUST_HOST
#ifdef ALLOW_THRUST_HOST
void h_xyzw_frequency_thrust(int * count, char * text, int n)
{
	const char letters[] = {'x','y','z','w'};

	*count = thrust::count_if(thrust::host, text, text + n,
			[&](char c)
			{
				for (const auto x : letters)
				{
					if (c == x)
					{
						return true;
					}
				}
				return false;
			});
}
#endif


std::vector<std::pair<std::chrono::time_point<std::chrono::high_resolution_clock>, int>> vtp;


void add_tic(int lbl)
{
	vtp.emplace_back(std::chrono::high_resolution_clock::now(), lbl);
}

/*
void add_tic(int cazz)
{
	std::cerr << "cazz " << cazz;
}*/

#define ADD_TIC add_tic(__LINE__);


void tictoc_results()
{
	std::cout << "TIC-TOCS [us]";
	for (int t=0; t<vtp.size()-1; t++)
	{
		const auto TOC = vtp[t+1].first, TIC = vtp[t].first;
		const auto TOCLBL = vtp[t+1].second, TICLBL = vtp[t].second;
		std::cout << "\n\t" << t << " - lines " << TICLBL << " to " << TOCLBL
				<< " -> " << std::chrono::duration_cast<std::chrono::microseconds>(TOC-TIC).count();
	}
	std::cout << std::endl;
}


int main(int argc, char ** argv)
{ 
	const char * FILENAME = sdkFindFilePath(argv[1], argv[0]);

	// find first CUDA device
	int devID = findCudaDevice(argc, (const char **)argv);

	// Get file size
	const auto FILE_SZ_BYTES = fsize(FILENAME);

	// Allocate buffers
	char * h_text = new char[FILE_SZ_BYTES];
	char * d_text;
	checkCudaErrors(cudaMalloc((void**)&d_text, FILE_SZ_BYTES));

	// Store file into buffer
	FILE  *fp = fopen(FILENAME, "r");
	if (fp == NULL)
	{
		printf("%s cannot find the input text file\n. Exiting..\n", argv[0]);
		return EXIT_FAILURE;
	}
	const int FILE_SZ_READ = fread(h_text, sizeof(char), FILE_SZ_BYTES, fp);
	fclose(fp);
	std::cout << "Read " << FILE_SZ_READ << " B from " << FILENAME << "(" << FILE_SZ_BYTES << " B)" << std::endl;

																ADD_TIC
	checkCudaErrors(cudaMemcpy(d_text, h_text, FILE_SZ_READ, cudaMemcpyHostToDevice));

	int count = 0;
	int * d_count;
																ADD_TIC
	checkCudaErrors(cudaMalloc(&d_count, sizeof(int)));									// NEGLIGIBLE
	checkCudaErrors(cudaMemset(d_count, 0, sizeof(int)));								// NEGLIGIBLE

	// GPU BURST
																ADD_TIC

	d_xyzw_frequency_ours<<<8, 256>>>(d_count, d_text, FILE_SZ_READ);					// NEGLIGIBLE

																ADD_TIC

	d_xyzw_frequency_thrust<<<1, 1>>>(d_count, d_text, FILE_SZ_READ);					// NEGLIGIBLE

																ADD_TIC

	checkCudaErrors(cudaMemcpy(&count, d_count, sizeof(int), cudaMemcpyDeviceToHost));	// HEAVY

																ADD_TIC

	h_xyzw_frequency_thrust(&count, h_text, FILE_SZ_READ);								// E+3 magnitude
																ADD_TIC

	tictoc_results();

	std::cout << "Counted " << count << " instances of 'x', 'y', 'z', or 'w' in \"" << FILENAME << "\"" << std::endl;

	checkCudaErrors(cudaFree(d_count));
	checkCudaErrors(cudaFree(d_text));
	delete[] h_text;

	return EXIT_SUCCESS;
}
