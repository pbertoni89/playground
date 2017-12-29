#include "ImageOclApplication.hpp"

ImageOclApplication::ImageOclApplication() :
	OclApplication("image.cl", "Filter", "-D FILTER_SIZE=1")
{
}

ImageOclApplication::~ImageOclApplication()
{
}

Image ImageOclApplication::LoadImage(const char* path)
{
	std::ifstream in(path, std::ios::binary);

	std::string s;
	in >> s;

	if (s != "P6")
	{
		std::runtime_error("Cannot load image");
	}

	// Skip comments
	for (;;)
	{
		getline(in, s);

		if (s.empty())
		{
			continue;
		}

		if (s[0] != '#')
		{
			break;
		}
	}

	std::stringstream str(s);
	int width, height, maxColor;
	str >> width >> height;
	in >> maxColor;

	if (maxColor != 255)
	{
		throw std::runtime_error("Image malformed: maxColor = " + std::to_string(maxColor));
	}

	{
		// Skip until end of line
		std::string tmp;
		getline(in, tmp);
	}

	std::vector<char> data(width * height * 3);
	in.read(reinterpret_cast<char*> (data.data()), data.size());

	const Image img = { data, width, height };
	return img;
}

void ImageOclApplication::SaveImage(const Image& img, const char* path)
{
	std::ofstream out(path, std::ios::binary);

	out << "P6\n";
	out << img.width << " " << img.height << "\n";
	out << "255\n";
	out.write(img.pixel.data(), img.pixel.size());
}

Image ImageOclApplication::RGBtoRGBA(const Image& input)
{
	Image result;
	result.width = input.width;
	result.height = input.height;

	for (std::size_t i = 0; i < input.pixel.size(); i += 3)
	{
		result.pixel.push_back(input.pixel[i + 0]);
		result.pixel.push_back(input.pixel[i + 1]);
		result.pixel.push_back(input.pixel[i + 2]);
		result.pixel.push_back(0);
	}

	return result;
}

Image ImageOclApplication::RGBAtoRGB(const Image& input)
{
	Image result;
	result.width = input.width;
	result.height = input.height;

	for (std::size_t i = 0; i < input.pixel.size(); i += 4)
	{
		result.pixel.push_back(input.pixel[i + 0]);
		result.pixel.push_back(input.pixel[i + 1]);
		result.pixel.push_back(input.pixel[i + 2]);
	}

	return result;
}

void ImageOclApplication::run()
{
	
	// Simple Gaussian blur filter
	float filter[] =
	{
		1, 2, 1,
		2, 4, 2,
		1, 2, 1
	};

	// Normalize the filter
	for (int i = 0; i < 9; ++i)
	{
		filter[i] /= 16.0f;
	}

	// OpenCL only supports RGBA, so we need to convert here
	_image_rgba = RGBtoRGBA(LoadImage("test.ppm"));

	/*
		OpenCL is well suited for this problems as it has direct support for images.
		Anything you would expect on images just works:
			You can read and write any pixel directly,
			the data can be automatically converted and
			they can be sampled using a bilinear filter.

		Creating images is just the same as with buffers,
		but it requires an additional parameter to describe the format of the image:
	*/

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateImage2D.html
	static const cl_image_format format = { CL_RGBA, CL_UNORM_INT8 };
	cl_mem inputImage = clCreateImage2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &format,
		_image_rgba.width, _image_rgba.height, 0,
		// This is a bug in the spec
		const_cast<char*> (_image_rgba.pixel.data()),
		&error);
	CheckError(error);

	cl_mem outputImage = clCreateImage2D(context, CL_MEM_WRITE_ONLY, &format,
		_image_rgba.width, _image_rgba.height, 0,
		nullptr, &error);
	CheckError(error);

	/*
		This is the OpenCL 1.1 API. In OpenCL 1.2, you would use clCreateImage which 
		uses an image descriptor so only one function is necessary instead of providing 5 different overloads.
		
		The important part here is the format, in particular, the second part: CL_UNORM_INT8.
		This indicates that the data is provided as 8-bit integers, and is stored as unsigned-normalized or unorm for short.
		Unsigned-normalized means that 0 maps to 0.0, and 255 maps to 1.0 when the data is read.
		It also requires you to read the data as floating point using read_imagef.
		This is perfect for our use case, as we have to weight each sample using a floating point value.
	*/

	// Create a buffer for the filter weights
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateBuffer.html
	cl_mem filterWeightsBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(float) * 9, filter, &error);
	CheckError(error);

	// Setup the kernel arguments
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputImage);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &filterWeightsBuffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &outputImage);

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateCommandQueue.html
	queue = clCreateCommandQueue(context, deviceIds[0], 0, &error);
	CheckError(error);

	// Run the processing
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clEnqueueNDRangeKernel.html
	std::size_t offset[3] = { 0 };
	std::size_t size[3] = { size_t(_image_rgba.width), size_t(_image_rgba.height), 1 };
	CheckError(clEnqueueNDRangeKernel(queue, kernel, 2, offset, size, nullptr, 0, nullptr, nullptr));

	// Prepare the result image, set to black
	Image result = _image_rgba;
	std::fill(result.pixel.begin(), result.pixel.end(), 0);

	// Get the result back to the host
	std::size_t origin[3] = { 0 };
	std::size_t region[3] = { size_t(result.width), size_t(result.height), 1 };
	clEnqueueReadImage(queue, outputImage, CL_TRUE,
		origin, region, 0, 0,
		result.pixel.data(), 0, nullptr, nullptr);

	SaveImage(RGBAtoRGB(result), "output.ppm");

	clReleaseMemObject(outputImage);
	clReleaseMemObject(filterWeightsBuffer);
	clReleaseMemObject(inputImage);

	clReleaseCommandQueue(queue);

	clReleaseKernel(kernel);
	clReleaseProgram(program);

	clReleaseContext(context);
}
