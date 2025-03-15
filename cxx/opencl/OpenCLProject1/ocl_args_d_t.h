#include "utils.h"

#include "CL\cl.h"

// Macros for OpenCL versions
#define OPENCL_VERSION_1_2  1.2f
#define OPENCL_VERSION_2_0  2.0f

/* Convenient container for all OpenCL specific objects used in the sample
*
* It consists of two parts:
*   - regular OpenCL objects which are used in almost each normal OpenCL applications
*   - several OpenCL objects that are specific for this particular sample
*
* You collect all these objects in one structure for utility purposes
* only, there is no OpenCL specific here: just to avoid global variables
* and make passing all these arguments in functions easier.
*/
struct ocl_args_d_t
{
	ocl_args_d_t();
	~ocl_args_d_t();

	// Regular OpenCL objects:
	cl_context       context;           // hold the context handler
	cl_device_id     device;            // hold the selected device handler
	cl_command_queue commandQueue;      // hold the commands-queue handler
	cl_program       program;           // hold the program handler
	cl_kernel        kernel;            // hold the kernel handler
	float            platformVersion;   // hold the OpenCL platform version (default 1.2)
	float            deviceVersion;     // hold the OpenCL device version (default. 1.2)
	float            compilerVersion;   // hold the device OpenCL C version (default. 1.2)

	// Objects that are specific for algorithm implemented in this sample
	cl_mem           srcA;              // hold first source buffer
	cl_mem           srcB;              // hold second source buffer
	cl_mem           dstMem;            // hold destination buffer
};
