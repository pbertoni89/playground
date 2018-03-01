#include "PseudoProducer.hpp"


void sample_parallel_for(float a[], size_t n)
{
	/**
	 * The grainsize sets a minimum threshold for parallelization.
	 * The parallel_for in the example invokes ApplyFoo::operator() on chunks,
	 * possibly of different sizes. Let chunksize be the number of iterations in a chunk.
	 * Using simple_partitioner guarantees that ⌈G/2⌉ ≤ chunksize ≤ G.
	 */
	const size_t GRAINSIZE = 8;

	FooTask pp(a);

	parallel_for(tbb::blocked_range<size_t>(0, n, GRAINSIZE), pp, tbb::simple_partitioner());

	pp.meta_destructor();
}


class BasicOperation
{
	const float _value;

public:
	BasicOperation(float value) :
		_value(value)
	{}

	float get_result() const
	{
		return _value * 3;
	}
};


void FooTask::read(float v, int pos) const
{
	_pOutput[pos] = v * 2;
	// CANNOT CHANGE MEMBERS (but what about _pOutput ???) // _iCounter ++;

	std::stringstream ss;
	ss << "read(): v = " << v << "\tI = " << _pInput[pos] << "\tO = " << _pOutput[pos] << "\n";
	std::cerr << ss.str();
}


void FooTask::write(float v, int pos)
{
	_pOutput[pos] = v * 2;
}


void FooTask::operator()(const tbb::blocked_range<size_t> & r) const
{
	for (size_t i = r.begin(); i != r.end(); ++i)
	{
		read(_pInput[i], (int)i);
		// CANNOT write(_pInput[i], i);
		_pFooStruct->a ++;
		*(_piCounterModifyable) = *(_piCounterModifyable) + 1;

		BasicOperation pc(_pInput[i]);
		std::stringstream ss;
		ss << "Pixel: " << pc.get_result() << std::endl;
		std::cerr << ss.str();
	}
}


FooTask::FooTask(float a[]) :
	_pInput(a),
	_piCounterModifyable((int*) calloc(sizeof(int), 1)),
	_iCounterNotModifyable(0),
	_pFooStruct((foo_struct*) calloc(sizeof(foo_struct), 1)),
	_pOutput(new float[4])
{
	std::stringstream ss;
	ss << "CTOR" << std::endl;
	std::cerr << ss.str();
}


FooTask::~FooTask()
{
	std::stringstream ss;
	ss << "~ _piCounterModifyable = " << *_piCounterModifyable << "\t_pFooStruct->a = " << _pFooStruct->a << std::endl;
	std::cerr << ss.str();
	//free(_piCounterModifyable);
	//free(_pFooStruct);
	//delete[] _pOutput;
}


void FooTask::meta_destructor()
{
	free(_piCounterModifyable);
	free(_pFooStruct);
	delete[] _pOutput;
}
