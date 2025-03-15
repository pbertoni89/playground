#define PROGRAM_FILE "add_numbers.cl"
#define KERNEL_FUNC "add_numbers"

#include <iomanip>
#include <chrono>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
#include <stdexcept>

#ifdef MAC
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif


/**
 * Find a GPU or CPU associated with the first available platform
 */
cl_device_id
create_device()
{
	cl_platform_id platform;
	cl_device_id dev;
	int err;

	// Identify a platform
	err = clGetPlatformIDs(1, &platform, nullptr);
	if (err < 0)
	{
		throw std::runtime_error("Couldn't identify a platform");
	}

	// Access a device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, nullptr);
	if (err == CL_DEVICE_NOT_FOUND)
	{
		printf("No GPU device available...\n");
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, nullptr);
	}
	if (err < 0) 
	{
		throw std::runtime_error("Couldn't access any devices");
	}
	return dev;
}


/**
 * Create program from a file and compile it
 */
cl_program
build_program(cl_context ctx, cl_device_id dev, const char * filename)
{
	cl_program program;
	FILE * program_handle;
	char *program_buffer, *program_log;
	size_t program_size, log_size;
	int err;

	// Read program file and place content into buffer
	program_handle = fopen(filename, "r");
	if (program_handle == nullptr)
	{
		throw std::runtime_error("Couldn't find the program file");
	}

	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char*) malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	auto sz_read = fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);

	// Create program from file
	program = clCreateProgramWithSource(ctx, 1, 
										(const char**) &program_buffer, &program_size, &err);
	if (err < 0)
	{
		throw std::runtime_error("Couldn't create the program");
	}

	// Build program
	err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
	if (err < 0)
	{
		// Find size of log and print to std output
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
		program_log = (char*) malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,  log_size + 1, program_log, nullptr);
		throw std::runtime_error(program_log);
	}

	free(program_buffer);
	return program;
}


/*
 * later their addresses will be passed as const size_t *
 * a school (device) containing
 * eight students divided into two classrooms of four students each
 *
 * array	local	global
 * 64		4		8
 * 128		4		16
 * 128		8		16
 * @param szArray
 * @param szLocal
 * @param szGlobal
 * @return szGroups
 */
size_t get_optimal_size(int argc, char ** argv, size_t & szArray, size_t & szLocal, size_t & szGlobal)
{
	/*
	 * Each kernel is adding two float4, so it actually spans over 8 floats
	 */
	constexpr size_t SZ_WORK = (sizeof(cl_float4) / sizeof(cl_float)) * 2;

	/*
	 * EMPYRIC boundary for OUR device
	 */
	const size_t SZ_LOCAL_MAX = 1024;

	static_assert(SZ_WORK == 8, "each kernel is working on exactly 8 floats");

	if (argc < 2)
	{
		throw std::runtime_error("usage: " + std::string(argv[0]) + " array_power_size [local_size global_size]");
	}

	szArray = std::pow(2., std::atoi(argv[1]));
	if (argc > 2)
	{
		szLocal = std::atoi(argv[2]);
		szGlobal = std::atoi(argv[3]);
	}
	else // auto "optimal?" setup. Start halving local size from global
	{
		szGlobal = szArray / SZ_WORK;
		szLocal = szGlobal;
		do
		{
			std::cout << "sz local " << szLocal << " /= 2 ->";
			szLocal /= 2;
			std::cout << szLocal << "\n";
		}
		while (szLocal > SZ_LOCAL_MAX);
	}

	const size_t szGroups = szGlobal / szLocal;
	std::cout << "add_numbers with array size " << szArray << ". local size "
		<< szLocal << ", global size " << szGlobal << ", groups " << szGroups << std::endl;

	return szGroups;
}


int main(int argc, char ** argv)
{
	srand(time(NULL));

	// OpenCL structures
	cl_device_id device;
	cl_context context;
	cl_program program;
	cl_kernel kernel;
	cl_command_queue queue;

	// i.e. int32
	cl_int err;
	size_t i, j;

	size_t szArray, szLocal, szGlobal;
	const size_t szGroups = get_optimal_size(argc, argv, szArray, szLocal, szGlobal);

	// Data and buffers
	std::vector<float> vf(szArray);
	float sum[szGroups];
	cl_mem buffer_input, buffer_output;

	// Initialize data. Simple progression sound too optimizable
	for (i=0; i<szArray; i++)
	{
		vf[i] = 1.0f * i * std::rand();
	}

	// Create device and context
	device = create_device();
	context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
	if (err < 0)
	{
		throw std::runtime_error("Couldn't create a context");
	}

	// Build program
	program = build_program(context, device, PROGRAM_FILE);

				const auto t_cl_setup = std::chrono::high_resolution_clock::now();

	// Create a command queue
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (err < 0)
	{
		throw std::runtime_error("Couldn't create a command queue");
	}

	// Create a kernel
	kernel = clCreateKernel(program, KERNEL_FUNC, &err);
	if (err < 0)
	{
		throw std::runtime_error("Couldn't create a kernel");
	}

	// Create data buffer
	buffer_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		szArray * sizeof(float), vf.data(), &err);
	buffer_output = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		szGroups * sizeof(float), sum, &err);

	if (err < 0)
	{
		throw std::runtime_error("Couldn't create a buffer");
	}

	// Create kernel arguments
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_input);      // __global float4 * data
	err |= clSetKernelArg(kernel, 1, szLocal * sizeof(float), nullptr);  // __local  float  * local_result
	err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_output);    // __global float  * group_result
	if (err < 0)
	{
		throw std::runtime_error("Couldn't create a kernel argument");
	}

				const auto t_cl_start = std::chrono::high_resolution_clock::now();

	// Enqueue kernel
	err = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &szGlobal, &szLocal, 0, nullptr, nullptr);
	if (err < 0)
	{
		throw std::runtime_error("Couldn't enqueue the kernel");
	}

	// Read the kernel's output. sum will contain local results
	err = clEnqueueReadBuffer(queue, buffer_output, CL_TRUE, 0, 
								sizeof(sum), sum, 0, nullptr, nullptr);
	if (err < 0)
	{
		throw std::runtime_error("Couldn't read the kernel buffer");
	}

	// Sum aggregation
	float sum_total = 0.0f;
	for (j=0; j<szGroups; j++)
	{
		sum_total += sum[j];
	}
				const auto t_cl_end = std::chrono::high_resolution_clock::now();

	// Reference implementation
	float sum_reference = 0.0f;
	for (i=0; i<szArray; i++)
	{
		sum_reference += vf[i];
	}
				const auto t_se_end = std::chrono::high_resolution_clock::now();

	// Check result (compare against Gauss rule)
	// const float sum_correctness = (1.0f * szArray / 2) * (szArray - 1);
	const float sum_correctness = sum_reference;
	const int RV = (std::fabs(sum_total - sum_correctness) > 0.01 * std::fabs(sum_correctness));
	printf("All computation end, expected %.2f, got %.2f ... Test %s\n", sum_correctness, sum_total, (RV ? "failed" : "passed"));
	if (std::isnan(sum_total))
	{
		throw std::runtime_error("NaN");
	}
				const auto T_CL_SETUP = std::chrono::duration_cast<std::chrono::microseconds>(t_cl_start - t_cl_setup).count(),
							T_CL_RUN = std::chrono::duration_cast<std::chrono::microseconds>(t_cl_end - t_cl_start).count(),
							T_SE = std::chrono::duration_cast<std::chrono::microseconds>(t_se_end - t_cl_end).count();
				std::cout << "us elapsed: cl " << T_CL_SETUP << " + " << T_CL_RUN << " vs se " << T_SE << "\n";

	// Deallocate resources
	clReleaseKernel(kernel);
	clReleaseMemObject(buffer_output);
	clReleaseMemObject(buffer_input);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	return RV;
}
