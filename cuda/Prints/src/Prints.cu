/*
 ============================================================================
 Name        : Prints.cu
 Author      : Patrizio Bertoni
 Version     :
 Copyright   : Your copyright notice
 Description : CUDA compute reciprocals
 ============================================================================
 */

#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>


static void CheckCudaErrorAux(const char *, unsigned, const char *, cudaError_t);

#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)


/**
 * CUDA kernel that computes reciprocal values for a given vector
 */
__global__ void reciprocalKernel(float * data, unsigned vectorSize)
{
	const unsigned idx = blockIdx.x * blockDim.x + threadIdx.x;

	if (idx < vectorSize)
	{
		data[idx] = 1.0/data[idx];
	}
}


/**
 * CUDA kernel for prints
 */
__global__ void hw()
{
        printf("Hw %d\n", blockIdx.x);
}


/**
 * Host function that copies the data and launches the work on GPU
 */
float * gpuReciprocal(float * h_pfData, unsigned size)
{
	const size_t SZ = sizeof(float) * size;

	float * rc = new float[size];
	float * d_pfData;

	CUDA_CHECK_RETURN(cudaMalloc((void **) &d_pfData, SZ));
	CUDA_CHECK_RETURN(cudaMemcpy(d_pfData, h_pfData, SZ, cudaMemcpyHostToDevice));

	const int BLOCK_SIZE = 256;
	const int BLOC_COUNT = (size+BLOCK_SIZE-1)/BLOCK_SIZE;

	reciprocalKernel<<<BLOC_COUNT, BLOCK_SIZE>>> (d_pfData, size);

	CUDA_CHECK_RETURN(cudaMemcpy(rc, d_pfData, SZ, cudaMemcpyDeviceToHost));
	CUDA_CHECK_RETURN(cudaFree(d_pfData));

	return rc;
}


/**
 * Host function for reference
 */
float * cpuReciprocal(float * data, unsigned size)
{
	float * rc = new float[size];

	for (unsigned cnt = 0; cnt < size; ++cnt)
	{
		rc[cnt] = 1.0/data[cnt];
	}

	return rc;
}


void initialize(float *data, unsigned size)
{
	for (unsigned i = 0; i < size; ++i)
	{
		data[i] = .5 * (i+1);
	}
}


int main(void)
{
	const int WORK_SIZE = 65530;
	float * pfData = new float[WORK_SIZE];

	initialize(pfData, WORK_SIZE);

	const float * pfReciprocalCpu = cpuReciprocal(pfData, WORK_SIZE);
	const float * pfReciprocalGpu = gpuReciprocal(pfData, WORK_SIZE);

	const float fSumCpu = std::accumulate(pfReciprocalCpu, pfReciprocalCpu + WORK_SIZE, 0.0);
	const float fSumGpu = std::accumulate(pfReciprocalGpu, pfReciprocalGpu + WORK_SIZE, 0.0);

	// Verify the results
	std::cout << "fSumGpu = " << fSumGpu << " fSumCpu = " << fSumCpu <<std::endl;

	// Free
	delete[] pfData;
	delete[] pfReciprocalCpu;
	delete[] pfReciprocalGpu;

	// Prints
    hw<<<16, 1>>>();

    cudaDeviceSynchronize();

	return 0;
}


/**
 * Check the return value of the CUDA runtime API call and exit
 * the application if the call has failed.
 */
static void CheckCudaErrorAux(const char * file, unsigned line, const char * statement, cudaError_t err)
{
	if (err == cudaSuccess)
	{
		return;
	}
	else
	{
		std::cerr << statement <<" returned " << cudaGetErrorString(err) << "("<< err
				<< ") at " << file << ":" <<line << std::endl;
		exit (1);
	}
}

