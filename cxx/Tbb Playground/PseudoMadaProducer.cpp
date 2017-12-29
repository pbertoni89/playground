#include "PseudoMadaProducer.hpp"

void foo(float f)
{
	std::stringstream ss;
	ss << "foo(): " << f << std::endl;
	std::cerr << ss.str();
}

void serialApplyFoo(float a[], size_t n)
{
	for( size_t i=0; i!=n; ++i )
		foo(a[i]);
}

void parallelProduction(float a[], size_t n)
{
//	PseudoMadaProducer pseudoMadaProducer(a);
	parallel_for(tbb::blocked_range<size_t>(0, n), PseudoMadaProducer(a));
//	pseudoMadaProducer.meta_destructor();
}

void PseudoMadaProducer::read(float v, int pos) const
{
	_pOutput[pos] = v * 2;
	// CANNOT CHANGE MEMBERS (but what about _pOutput ???) // _iCounter ++;

	std::stringstream ss;
	ss << "read(): v = " << v << "\tI = " << _pInput[pos] << "\tO = " << _pOutput[pos] << std::endl;
	std::cerr << ss.str();
}

void PseudoMadaProducer::write(float v, int pos)
{
	_pOutput[pos] = v * 2;
}

void PseudoMadaProducer::operator()(const tbb::blocked_range<size_t>& r) const
{
	for (size_t i = r.begin(); i != r.end(); ++i)
	{
		float *_pInputCopy = _pInput;
		foo(_pInputCopy[i]);

		read(_pInput[i], (int)i);
		// CANNOT write(_pInput[i], i);
		_pFooStruct->a ++;
		*(_piCounterModifyable) = *(_piCounterModifyable) + 1;

		PixelComputation * _pTempPixelComputation = new PixelComputation(_pInput[i]);
		std::stringstream ss;
		ss << "Pixel: " << _pTempPixelComputation->get_result() << std::endl;
		std::cerr << ss.str();
		delete(_pTempPixelComputation);
	}
}

PseudoMadaProducer::PseudoMadaProducer(float a[]) :
	_pInput(a),
	_piCounterModifyable((int*)calloc(sizeof(int), 1)),
	_iCounterNotModifyable(0),
	_pFooStruct((FooStruct*)calloc(sizeof(FooStruct), 1)),
	_pOutput(new float[4])
{
	std::stringstream ss;
	ss << "CTOR" << std::endl;
	std::cerr << ss.str();
}

PseudoMadaProducer::~PseudoMadaProducer()
{
	std::stringstream ss;
	ss << "~ _piCounterModifyable = " << *_piCounterModifyable << "\t_pFooStruct->a = " << _pFooStruct->a << std::endl;
	std::cerr << ss.str();
}

void PseudoMadaProducer::meta_destructor()
{
	free(_piCounterModifyable);
	free(_pFooStruct);
	delete _pOutput;
}
