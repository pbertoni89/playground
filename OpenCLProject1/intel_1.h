#pragma once

#include "utils.h"
#include "ocl_args_d_t.h"

#include "CL\cl.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <memory.h>
#include <vector>
//for perf. counters
#include <Windows.h>

/*
* Check whether an OpenCL platform is the required platform
* (based on the platform's name)
*/
bool CheckPreferredPlatformMatch(cl_platform_id platform, const char* preferredPlatform);

/*
* Find and return the preferred OpenCL platform
* In case that preferredPlatform is NULL, the ID of the first discovered platform will be returned
*/
cl_platform_id FindOpenCLPlatform(const char* preferredPlatform, cl_device_type deviceType);

/*
* This function read the OpenCL platdorm and device versions
* (using clGetxxxInfo API) and stores it in the ocl structure.
* Later it will enable us to support both OpenCL 1.2 and 2.0 platforms and devices
* in the same program.
*/
int GetPlatformAndDeviceVersion(cl_platform_id platformId, ocl_args_d_t *ocl);

/*
* Generate random value for input buffers
*/
void generateInput(cl_int* inputArray, cl_uint arrayWidth, cl_uint arrayHeight);

/*
* This function picks/creates necessary OpenCL objects which are needed.
* The objects are:
* OpenCL platform, device, context, and command queue.
*
* All these steps are needed to be performed once in a regular OpenCL application.
* This happens before actual compute kernels calls are performed.
*
* For convenience, in this application you store all those basic OpenCL objects in structure ocl_args_d_t,
* so this function populates fields of this structure, which is passed as parameter ocl.
* Please, consider reviewing the fields before going further.
* The structure definition is right in the beginning of this file.
*/
int SetupOpenCL(ocl_args_d_t *ocl, cl_device_type deviceType);

/*
* Create and build OpenCL program from its source code
*/
int CreateAndBuildProgram(ocl_args_d_t *ocl);

/*
* Create OpenCL buffers from host memory
* These buffers will be used later by the OpenCL kernel
*/
int CreateBufferArguments(ocl_args_d_t *ocl, cl_int* inputA, cl_int* inputB, cl_int* outputC, cl_uint arrayWidth, cl_uint arrayHeight);

/*
* Set kernel arguments
*/
cl_uint SetKernelArguments(ocl_args_d_t *ocl);

/*
* Execute the kernel
*/
cl_uint ExecuteAddKernel(ocl_args_d_t *ocl, cl_uint width, cl_uint height);

/*
* "Read" the result buffer (mapping the buffer to the host memory address)
*/
bool ReadAndVerify(ocl_args_d_t *ocl, cl_uint width, cl_uint height, cl_int *inputA, cl_int *inputB);

/*
* main execution routine
* Basically it consists of three parts:
*   - generating the inputs
*   - running OpenCL kernel
*   - reading results of processing
*/
int do_intel_1();
