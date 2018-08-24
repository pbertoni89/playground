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

/**
 * Vector addition: C = A + B.
 *
 * This sample is a very basic sample that implements element by element
 * vector addition. It is the same as the sample illustrating Chapter 2
 * of the programming guide with some additions like error checking.
 */

#include <stdio.h>
#include <iostream>
#include <chrono>

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>
#include <helper_cuda.h>


/**
 * CUDA Kernel Device code
 *
 * Computes the vector addition of A and B into C. The 3 vectors have the same
 * number of elements numElements.
 */
__global__ void
d_vector_add(const float * A, const float * B, float * C, int numElements)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < numElements)
	{
		C[i] = A[i] + B[i];
	}
}


void print_last_error(const std::string & MSG, cudaError_t & err)
{
	std::cerr << "Failed to " << MSG << ", error code: " << cudaGetErrorString(err) << std::endl;
}


/**
 * Host main routine
 */
int main(void)
{
	// Problem parameters
	const int numElements = 500000 * 128;
	const size_t threadsPerBlock = 32;
	const size_t blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;

	// Error code to check return values for CUDA calls
	cudaError_t err = cudaSuccess;

	// Print the vector length to be used, and compute its size
	const size_t size = numElements * sizeof(float);
	std::cout << "Vector addition of " << numElements << " elements" << std::endl;

	// Allocate the host input vector A, B, C
	float	* h_A = (float *) malloc(size),
			* h_B = (float *) malloc(size),
			* h_C = (float *) malloc(size);

	// Verify that allocations succeeded
	if (h_A == nullptr or h_B == nullptr or h_C == nullptr)
	{
		std::cerr << "Failed to allocate host vectors!" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Initialize the host input vectors
	for (int i = 0; i < numElements; ++i)
	{
		h_A[i] = rand() / (float)RAND_MAX;
		h_B[i] = rand() / (float)RAND_MAX;
	}

	// Allocate the device input vectors A, B, C
	float * d_A = nullptr, * d_B = nullptr, * d_C = nullptr;

	err = cudaMalloc((void **)&d_A, size);
	if (err != cudaSuccess)
	{
		print_last_error("allocate device vector A", err);
		exit(EXIT_FAILURE);
	}

	err = cudaMalloc((void **)&d_B, size);
	if (err != cudaSuccess)
	{
		print_last_error("allocate device vector B", err);
		exit(EXIT_FAILURE);
	}

	err = cudaMalloc((void **)&d_C, size);
	if (err != cudaSuccess)
	{
		print_last_error("allocate device vector C", err);
		exit(EXIT_FAILURE);
	}


	const auto T0 = std::chrono::high_resolution_clock::now();
	// Input: copy the host vectors A and B from host memory to the device vectors in device memory
	std::cout << "Copy input data from the host memory to the CUDA device" << std::endl;

	err = cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
	if (err != cudaSuccess)
	{
		print_last_error("copy vector A from host to device", err);
		exit(EXIT_FAILURE);
	}

	err = cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
	if (err != cudaSuccess)
	{
		print_last_error("copy vector B from host to device", err);
		exit(EXIT_FAILURE);
	}


	// Launch the Vector Add CUDA Kernel
	std::cout << "CUDA kernel launch with " << blocksPerGrid << " blocks of "
		<< threadsPerBlock << " threads each" << std::endl;

	d_vector_add<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, numElements);

	err = cudaGetLastError();
	if (err != cudaSuccess)
	{
		print_last_error("launch vectorAdd kernel", err);
		exit(EXIT_FAILURE);
	}


	// Output: copy the device vector in device memory to the host vector in host memory
	std::cout << "Copy output data from the CUDA device to the host memory" << std::endl;

	err = cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
	if (err != cudaSuccess)
	{
		print_last_error("copy vector C from device to host", err);
		exit(EXIT_FAILURE);
	}
	const auto T1 = std::chrono::high_resolution_clock::now();

	// Verify that the result vector is correct
	for (int i = 0; i < numElements; ++i)
	{
		if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5)
		{
			std::cerr << "Result verification failed at element " << i << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	const auto T2 = std::chrono::high_resolution_clock::now();

	const auto T01 = std::chrono::duration_cast<std::chrono::microseconds>(T1 - T0).count(),
				T12 = std::chrono::duration_cast<std::chrono::microseconds>(T2 - T1).count();

	std::cout << "Test PASSED: " << T01 << " vs " << T12 << " microseconds" << std::endl;


	// Free device global memory
	err = cudaFree(d_A);
	if (err != cudaSuccess)
	{
		print_last_error("free device vector A", err);
		exit(EXIT_FAILURE);
	}

	err = cudaFree(d_B);
	if (err != cudaSuccess)
	{
		print_last_error("free device vector B", err);
		exit(EXIT_FAILURE);
	}

	err = cudaFree(d_C);
	if (err != cudaSuccess)
	{
		print_last_error("free device vector C", err);
		exit(EXIT_FAILURE);
	}

	// Free host memory
	free(h_A);
	free(h_B);
	free(h_C);

	std::cout << "program end with 0";
	return 0;
}

