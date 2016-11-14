#pragma once

#include "utils.h"

#include <chrono>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define USER_CHOOSE

class OclApplication
{
	void _set_platform();
	void _set_device();
	void _set_context();
	void _set_kernel();

	std::string LoadKernel();
	void CreateProgram(const std::string& source);

	static std::string GetPlatformName(cl_platform_id id);
	static std::string GetDeviceName(cl_device_id id);	

protected:
	cl_uint platformIdCount;
	std::vector<cl_platform_id> platformIds;
	cl_uint deviceIdCount;
	std::vector<cl_device_id> deviceIds;
	/*const*/ cl_context_properties contextProperties[4];

	int _iPlatform;
	std::string _sPlatformName;

	int _iDevice;
	std::string _sDeviceName;

	/** last error found */
	cl_int error;
	cl_context context;
	cl_program program;
	cl_kernel kernel;
	cl_command_queue queue;

	std::string _kernel_filepath;
	std::string _kernel_funcname;
	std::string _kernel_params;

	void CheckError();
	static void CheckError(cl_int error);

public:
	OclApplication(std::string kernel_filepath, std::string _kernel_funcname, std::string _kernel_params);
	virtual ~OclApplication();
	virtual void run() = 0;
};
