/*
	The second part that is necessary for reading an image is a sampler.
	It describes how coordinates are interpreted and whether the image data should be filtered.
	
	In our case,
		we want to index pixels using integers, so we have to set CLK_NORMALIZED_COORDS_FALSE,
		we want out-of-bounds accesses to be clamped (CLK_ADDRESS_CLAMP_TO_EDGE)
		we don’t want any filtering. CLK_FILTER_NEAREST means that the sampler
			should return the value of the nearest pixel to the requested pixel, without any interpolation.
*/

/*
	OpenCL has multiple address spaces. In the second example, we only used __global.
	__constant is another address space where you can store read-only data.
	From the host side, it looks exactly the same as global memory, but some devices like GPUs
	do actually have special support for constant data which may result in better performance.
	That’s also the reason why it can be fairly small. OpenCL only guarantees that it is at least 64 KiB in size.
*/
__constant sampler_t sampler =
CLK_NORMALIZED_COORDS_FALSE
| CLK_ADDRESS_CLAMP_TO_EDGE
| CLK_FILTER_NEAREST;

#define FILTER_SIZE 5

float FilterValue(__constant const float* filterWeights,
	const int x, const int y)
{
	return filterWeights[(x + FILTER_SIZE) + (y + FILTER_SIZE)*(FILTER_SIZE * 2 + 1)];
}

__kernel void Filter(
	__read_only image2d_t input,
	__constant float* filterWeights,
	__write_only image2d_t output)
{

	const int2 pos =
	{
		get_global_id(0),
		get_global_id(1)
	};

	float4 sum = (float4)(0.0f);
	for (int y = -FILTER_SIZE; y <= FILTER_SIZE; y++)
	{
		for (int x = -FILTER_SIZE; x <= FILTER_SIZE; x++)
		{
			sum +=	FilterValue(filterWeights, x, y)
					* read_imagef(input, sampler, pos + (int2)(x, y));
		}
	}

	write_imagef(output, (int2)(pos.x, pos.y), sum);
}
