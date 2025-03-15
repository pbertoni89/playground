#include "OclApplication.hpp"

OclApplication::OclApplication(std::string kernel_filepath, std::string kernel_funcname, std::string kernel_params) :
	platformIdCount(0),
	_iPlatform(0),
	deviceIdCount(0),
	_kernel_filepath(kernel_filepath),
	_kernel_funcname(kernel_funcname),
	_kernel_params(kernel_params),
	error(CL_SUCCESS)
{
	_set_platform();
	_set_device();
	_set_context();
	_set_kernel();
}

OclApplication::~OclApplication()
{
}

void OclApplication::_set_platform()
{
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetPlatformIDs.html
	clGetPlatformIDs(0, nullptr, &platformIdCount);

	if (platformIdCount == 0)
		throw std::runtime_error("No OpenCL platform found");
	else
		std::cout << "Found " << platformIdCount << " platform(s)" << std::endl;

	platformIds = std::vector<cl_platform_id>(platformIdCount);
	clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);

	for (cl_uint i = 0; i < platformIdCount; ++i)
		std::cout << "\t (" << i << ") : " << GetPlatformName(platformIds[i]) << std::endl;

#ifdef USER_CHOOSE
	std::cout << "Select the preferred one: ";
	std::cin >> _iPlatform;

	if (_iPlatform < 0 || _iPlatform > platformIdCount)
		exit(1);
#else
	_iPlatform = platformIdCount - 1;
#endif
	_sPlatformName = GetPlatformName(platformIds[_iPlatform]);
}

void OclApplication::_set_device()
{
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceIDs.html
	clGetDeviceIDs(platformIds[_iPlatform], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceIdCount);

	if (deviceIdCount == 0)
		throw std::runtime_error("No OpenCL devices found on " + _sPlatformName);
	else
		std::cout << "Found " << deviceIdCount << " device(s) on platform " << _sPlatformName << std::endl;

	deviceIds = std::vector<cl_device_id>(deviceIdCount);
	clGetDeviceIDs(platformIds[_iPlatform], CL_DEVICE_TYPE_ALL, deviceIdCount, deviceIds.data(), nullptr);

	for (cl_uint i = 0; i < deviceIdCount; ++i)
		std::cout << "\t (" << i << ") : " << GetDeviceName(deviceIds[i]) << std::endl;

#ifdef USER_CHOOSE
	std::cout << "Select the preferred one: ";
	std::cin >> _iDevice;
	if (_iDevice < 0 || _iDevice > deviceIdCount)
		exit(1);
#else
	_iDevice = deviceIdCount - 1;
#endif
	_sDeviceName = GetDeviceName(deviceIds[_iDevice]);
}

void OclApplication::_set_context()
{
	contextProperties[0] = CL_CONTEXT_PLATFORM;
	contextProperties[1] = reinterpret_cast<cl_context_properties> (platformIds[_iPlatform]);
	contextProperties[2] = 0;
	contextProperties[3] = 0;

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateContext.html
	context = clCreateContext(contextProperties, deviceIdCount, deviceIds.data(), nullptr, nullptr, &error);
	CheckError(error);
}

void OclApplication::_set_kernel()
{
	CreateProgram(LoadKernel());

	if (_kernel_params.size())
		CheckError(clBuildProgram(program, deviceIdCount, deviceIds.data(), _kernel_params.c_str(), nullptr, nullptr));
	else
		CheckError(clBuildProgram(program, deviceIdCount, deviceIds.data(), nullptr, nullptr, nullptr));

	kernel = clCreateKernel(program, _kernel_funcname.c_str(), &error);
	CheckError(error);
}

// old utils.cpp

std::string OclApplication::LoadKernel()
{
	std::ifstream in(_kernel_filepath);
	std::string result(
		(std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	return result;
}

void OclApplication::CreateProgram(const std::string& source)
{
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateProgramWithSource.html
	size_t lengths[1] = { source.size() };
	const char* sources[1] = { source.data() };

	cl_int error = 0;
	program = clCreateProgramWithSource(context, 1, sources, lengths, &error);
	CheckError(error);
}

std::string OclApplication::GetPlatformName(cl_platform_id id)
{
	size_t size = 0;
	clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, nullptr, &size);

	std::string result;
	result.resize(size);
	clGetPlatformInfo(id, CL_PLATFORM_NAME, size, const_cast<char*> (result.data()), nullptr);

	return result;
}

std::string OclApplication::GetDeviceName(cl_device_id id)
{
	size_t size = 0;
	clGetDeviceInfo(id, CL_DEVICE_NAME, 0, nullptr, &size);

	std::string result;
	result.resize(size);
	clGetDeviceInfo(id, CL_DEVICE_NAME, size, const_cast<char*> (result.data()), nullptr);

	return result;
}

void OclApplication::CheckError()
{
	OclApplication::CheckError(error);
}

void OclApplication::CheckError(cl_int error)
{
	if (error != CL_SUCCESS)
	{
		throw std::runtime_error("OpenCL call failed with error " + std::string(TranslateOpenCLError(error)));
	}
}
