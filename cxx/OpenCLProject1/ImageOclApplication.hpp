#pragma once

#include "OclApplication.hpp"

struct Image
{
	std::vector<char> pixel;
	int width, height;
};

class ImageOclApplication :
	public OclApplication
{
	size_t testDataSize;
	std::vector<float> a, b;
	cl_mem aBuffer, bBuffer;

	Image _image_rgba;

	
	Image LoadImage(const char* path);
	void SaveImage(const Image& img, const char* path);
	Image RGBtoRGBA(const Image& input);
	Image RGBAtoRGB(const Image& input);

public:
	ImageOclApplication();
	virtual ~ImageOclApplication();
	void run();
};
