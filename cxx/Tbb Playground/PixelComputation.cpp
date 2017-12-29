#include "PixelComputation.hpp"

PixelComputation::PixelComputation(float value)
{
	_value = value;
}

PixelComputation::~PixelComputation()
{
}

float PixelComputation::get_result()
{
	return _value * 3;
}
