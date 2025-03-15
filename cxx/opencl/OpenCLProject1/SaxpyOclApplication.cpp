#include "SaxpyOclApplication.hpp"

SaxpyOclApplication::SaxpyOclApplication() :
	OclApplication("saxpy.cl", "SAXPY", "")
{
	// Prepare some test data
	testDataSize = 1 << 10;
	a = std::vector<float>(testDataSize);
	b = std::vector<float>(testDataSize);
	for (int i = 0; i < testDataSize; ++i)
	{
		a[i] = static_cast<float> (23 ^ i);
		b[i] = static_cast<float> (42 ^ i);
	}
	std::cout << "Generated two arrays of size " << testDataSize << ". First elements" << std::endl;
	for (int i = 0; i < 10; i++)
		std::cout << "\t" << a[i];
	std::cout << std::endl;
	for (int i = 0; i < 10; i++)
		std::cout << "\t" << b[i];
	std::cout << std::endl;
}

SaxpyOclApplication::~SaxpyOclApplication()
{
}

void SaxpyOclApplication::run()
{
	aBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(float) * (testDataSize),
		a.data(), &error);
	CheckError();

	bBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof(float) * (testDataSize),
		b.data(), &error);
	CheckError();

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateCommandQueue.html
#ifdef CL_USE_DEPRECATED_OPENCL_1_2_APIS
	queue = clCreateCommandQueue(context, deviceIds[0], 0, &error);
#else
	cl_command_queue_properties props;
	queue = clCreateCommandQueueWithProperties(context, deviceIds[0], &props, &error);
#endif

	CheckError();

	/*
	The kernel is now ready to be used; all that is left is to bind the arguments to it.
	This is done using clSetKernelArg.
	With OpenCL 1.1, it’s not possible to find out which argument has which name, so we have to set them by number
	*/

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &aBuffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bBuffer);
	static const float two = 2.0f;
	clSetKernelArg(kernel, 2, sizeof(float), &two);

	/*
	As mentioned above, we have to specify the size of the vector before we can run the kernel.
	That is, we have to tell OpenCL what dimension our work domain has and the extents in each dimension
	*/

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clEnqueueNDRangeKernel.html
	const size_t globalWorkSize[] = { testDataSize, 0, 0 };
	CheckError(clEnqueueNDRangeKernel(queue, kernel, 1,
		nullptr,
		globalWorkSize,
		nullptr,
		0, nullptr, nullptr));

	/*
	Get the results back to the host
	We call it with the blocking parameter set to true,
	so the call will block until the kernel has finished working before the data is read back.

	We also didn't specify the work group size, so the implementation will use something suitable.
	*/

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clEnqueueReadBuffer.html
	CheckError(clEnqueueReadBuffer(queue, bBuffer, CL_TRUE, 0,
		sizeof(float) * testDataSize,
		b.data(),
		0, nullptr, nullptr));

	std::cout << "Reading results " << std::endl;
	for (int i = 0; i < 10; i++)
		std::cout << "\t" << b[i];
	std::cout << std::endl;


	clReleaseCommandQueue(queue);

	clReleaseMemObject(bBuffer);
	clReleaseMemObject(aBuffer);

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseContext(context);
}
