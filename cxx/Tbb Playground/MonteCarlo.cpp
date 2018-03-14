#include "/media/xnext/DATA/opt/intel/compilers_and_libraries_2018.1.163/linux/mkl/include/mkl_vsl.h"

#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"
#include "tbb/tick_count.h"


class PiCalculator {
public:
	long numpoints;
	long& in;
	VSLStreamStatePtr stream;

	PiCalculator(long numpoints, long& in, VSLStreamStatePtr stream) :
	numpoints(numpoints), in(in), stream(stream) {
	   in = 0; // make sure to initialize to zero.
	}

	void operator()() {
	const int block_size = 2048;
	double variates[2 * block_size];
	int nblocks, ntail, i, j;
	nblocks = numpoints / block_size;
	ntail = numpoints - nblocks * block_size;
	for (j = 0; j < nblocks; j++) {
	   vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 2 * block_size,variates, 0.0, 1.0);
	   for (i = 0; i < block_size; i++) {
		 double x = variates[2 * i + 0];
		 double y = variates[2 * i + 1];
		 if (x * x + y * y <= 1.0)
		 ++(in);
	   }
	}

	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 2 * ntail, variates,0.0, 1.0);

	for (i = 0; i < ntail; i++) {

	double x = variates[2 * i + 0];

	double y = variates[2 * i + 1];

	if (x * x + y * y <= 1.0)

	++(in);

	}

	};

};

int main() {

	int errorcode;

	const long samples = 10000000000l;

	int seed = 1;

	int tasks = 50;

	VSLStreamStatePtr stream[tasks];

	for (int i = 0; i < tasks; i++) {

	errorcode = vslNewStream(&stream[i], VSL_BRNG_MCG59, seed);

	if (errorcode) {

	return 1;

	}

	errorcode = vslLeapfrogStream(stream[i], i, tasks);

	if (errorcode) {
		return 1;
	}

	}

	tbb::task_scheduler_init init;

tbb::task_group group;

long results[tasks];

long samplesPerTasks = samples/tasks;

tbb::tick_count t0 = tbb::tick_count::now();

for (int i = 0; i < tasks; i++) {

group.run(PiCalculator(samplesPerTasks, results[i], stream[i]));

}

group.wait();

tbb::tick_count t1 = tbb::tick_count::now();

long result = 0;

for(int i=0;i

result += results[i];

}

std::cout << "pi = " << 4.0 * result / samples << std::endl;

std::cout << "time [s]: " << (t1-t0).seconds();

for (int i = 0; i < tasks; i++)

vslDeleteStream(&stream[i]);

return 0;
}