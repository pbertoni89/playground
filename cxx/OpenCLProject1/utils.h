#ifdef __APPLE__
#include "OpenCL/opencl.h"
#else
#include "CL/cl.h"
#endif

#include <d3d9.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#pragma once

// -- INTEL TUTORIAL

// Print useful information to the default output. Same usage as with printf
void LogInfo(const char* str, ...);

// Print error notification to the default output. Same usage as with printf
void LogError(const char* str, ...);

// Read OpenCL source code from fileName and store it in source. The number of read bytes returns in sourceSize
int ReadSourceFromFile(const char* fileName, char** source, size_t* sourceSize);

/* This function helps to create informative messages in
* case when OpenCL errors occur. It returns a string
* representation for an OpenCL error code.
* (E.g. "CL_DEVICE_NOT_FOUND" instead of just -1.) */
const char* TranslateOpenCLError(cl_int errorCode);
