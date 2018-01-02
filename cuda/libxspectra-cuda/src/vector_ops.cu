/**
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#include "../include/Performances.hpp"

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>
#include <helper_cuda.h>

#include <stdio.h>

#include <iostream>
#include <vector>


/**
 * CUDA Kernel Device code
 *
 * Computes the vector addition of A and B into C. The 3 vectors have the same
 * number of elements numElements.
 */
__global__
void
d_vector_add(const float * A, const float * B, float * C, int numElements)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < numElements)
	{
		C[i] = A[i] + B[i];
	}
}



class VectorTester
{
	const int m_numElements;
	const size_t SZ_VEC_BYTES;

	cudaError_t m_err;

	std::vector<float> h_A, h_B, h_C;

	float * d_A, * d_B, * d_C;

	std::string _describe(const std::string & prefix);

public:

	VectorTester(int numElements);

	/**
	 * Free device global memory
	 */
	~VectorTester();

	/**
	 * Initialize the host input vectors
	 *
	 * TODO overload with outer values
	 */
	void randomize();

	/**
	 * Launch the Vector Add CUDA Kernel
	 */
	void launch();

	/**
	 *  Copy the device result vector in device memory to the host result vector in host memory.
	 */
	void check();

	/**
	 * Copy the host input vectors A and B in host memory to the device input vectors in device memory
	 */
	void load_values();

	/**
	 * Compute vectorAdd in-thread, on-CPU.
	 */
	void cpu_benchmark();
};


VectorTester::VectorTester(int numElements) :
	m_numElements(numElements),
	SZ_VEC_BYTES(numElements * sizeof(float)),
	m_err(cudaSuccess),
	h_A(SZ_VEC_BYTES), h_B(SZ_VEC_BYTES), h_C(SZ_VEC_BYTES, 0),
	d_A(nullptr), d_B(nullptr), d_C(nullptr)
{
	std::cout << "[Vector addition of " << numElements << " elements]\n";

	// Allocate the device input vectors
	m_err = cudaMalloc((void **)&d_A, SZ_VEC_BYTES);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to allocate d_A"));
	}
	m_err = cudaMalloc((void **)&d_B, SZ_VEC_BYTES);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to allocate d_B"));
	}
	m_err = cudaMalloc((void **)&d_C, SZ_VEC_BYTES);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to allocate d_C"));
	}
}


VectorTester::~VectorTester()
{
	m_err = cudaFree(d_A);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to free device vector A"));
	}

	m_err = cudaFree(d_B);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to free device vector B"));
	}

	m_err = cudaFree(d_C);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to free device vector C"));
	}
}


void VectorTester::randomize()
{
	for (int i = 0; i < m_numElements; i++)
	{
		h_A[i] = rand()/(float)RAND_MAX;
		h_B[i] = rand()/(float)RAND_MAX;
	}
}


void VectorTester::load_values()
{
	printf("Copy input data from the host memory to the CUDA device\n");

	m_err = cudaMemcpy(d_A, h_A.data(), SZ_VEC_BYTES, cudaMemcpyHostToDevice);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to copy vector A from host to device"));
	}

	m_err = cudaMemcpy(d_B, h_B.data(), SZ_VEC_BYTES, cudaMemcpyHostToDevice);
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to copy vector B from host to device"));
	}
}


void VectorTester::check()
{
	printf("Copy output data from the CUDA device to the host memory\n");

	m_err = cudaMemcpy(h_C.data(), d_C, SZ_VEC_BYTES, cudaMemcpyDeviceToHost);

	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to copy vector C from device to host"));
	}

	// Verify that the result vector is correct
	for (int i = 0; i < m_numElements; ++i)
	{
		if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5)
		{
			throw std::runtime_error("Result verification failed at element %d " + std::to_string(i));
		}
	}

	printf("Test PASSED\n");
}


void VectorTester::launch()
{
	const int threadsPerBlock = 256;
	const int blocksPerGrid =(m_numElements + threadsPerBlock - 1) / threadsPerBlock;
	printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
	d_vector_add<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, m_numElements);

	m_err = cudaGetLastError();
	if (m_err != cudaSuccess)
	{
		throw std::runtime_error(_describe("Failed to launch vectorAdd kernel"));
	}
}


void VectorTester::cpu_benchmark()
{
	for (int i = 0; i < m_numElements; i++)
	{
		h_C[i] = h_A[i] + h_B[i];
	}
}


std::string VectorTester::_describe(const std::string & prefix)
{
	return prefix + ": " + std::string(cudaGetErrorString(m_err));
}



namespace pietra
{
	namespace cuda
	{
		/**
		 * Vector addition: C = A + B.
		 *
		 * This sample is a very basic sample that implements element by element
		 * vector addition. It is the same as the sample illustrating Chapter 2
		 * of the programming guide with some additions like error checking.
		 */
		int vector_test(int numElements)
		{
			pietra::utils::Performances performances;
			ADD_TIC

			VectorTester vTester(numElements);

			ADD_TIC

			vTester.randomize();

			ADD_TIC

			vTester.load_values();

			ADD_TIC

			vTester.launch();

			ADD_TIC

			vTester.cpu_benchmark();

			ADD_TIC

			vTester.check();

			ADD_TIC

			performances.tictoc_results();

			return 0;
		}
	}
}
