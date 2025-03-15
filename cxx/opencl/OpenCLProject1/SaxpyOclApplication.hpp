#pragma once
#include "OclApplication.hpp"

class SaxpyOclApplication :
	public OclApplication
{
	size_t testDataSize;
	std::vector<float> a, b;
	cl_mem aBuffer, bBuffer;

public:
	SaxpyOclApplication();
	virtual ~SaxpyOclApplication();
	void run();
};
