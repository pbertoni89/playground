/**
 * https://helloacm.com/simple-tutorial-with-openmp-how-to-use-parallel-block-in-cc-using-openmp/
 *
 * Build with
 * gcc -o hello_openmp hello_openmp.c -fopenmp
 *
 * CMake: add set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
 */

#include <stdio.h>
#include <array>
#include <omp.h>


void demo_2()
{
	int x = 1;
	int y = x + 2;
	int data;

	#pragma omp parallel num_threads(y * 3)
	{
		const int id = omp_get_thread_num();
		const int total = omp_get_num_threads();

		data = id; // threads may interleaving the modification, because data scope is on master thread
		printf("Greetings from process %d out of %d with Data %d\n", id, total, data);

		// make sure only 1 thread exectutes the critical section at a time
		#pragma omp critical
		{
			data = id; // threads may interleaving the modification
			printf("Critical from process %d out of %d with Data %d\n", id, total, data);
		}
	}

	printf("parallel for ends\n");
}


/**
 * The following example demonstrates why synchronization is difficult to perform correctly through variables.
 * The value of flag is undefined in both prints on thread 1 and the value of data is only well-defined in the second print.
 */
void demo_sync_difficult()
{
	int data, flag = 0;

	#pragma omp parallel num_threads(2)
	{
		const auto ID = omp_get_thread_num();
		if (ID==0)
		{
			data = 42;										// Write to the data buffer that will be read by thread
			#pragma omp flush(flag, data)					// Flush data to thread 1 and strictly order the write to datarelative to the write to the flag
			flag = 1;										// Set flag to release thread 1
			#pragma omp flush(flag)							// Flush flag to ensure that thread 1 sees the change
		}
		else if (ID==1)
		{
			#pragma omp flush(flag, data)
			while (flag < 1)								// Loop until we see the update to the flag
			{
				#pragma omp flush(flag, data)
			}
			printf("flag=%d data=%d\n", flag, data);		// Values of flag and data are undefined
			#pragma omp flush(flag, data)
			printf("flag=%d data=%d\n", flag, data);  		// Values data will be 42, value of flagstill undefined
		}
	}
}


// better not to be on the stack
constexpr size_t ARRSZ = 8192 * 512, TRIALS = 1;
std::array<float, ARRSZ> ar;

void subdomain(float * x, size_t istart, size_t ipoints)
{
	for (size_t i=0; i<ipoints; i++)
		x[istart + i] = rand();
}

void sub_par(float * x, size_t npoints)
{
	int iam, nt;
	size_t ipoints, istart;
	#pragma omp parallel default(shared) private(iam,nt,ipoints,istart)
	{
		iam = omp_get_thread_num();
		nt = omp_get_num_threads();
		ipoints = npoints / nt;
		istart = iam * ipoints;
		if (iam == nt - 1)
			ipoints = npoints - istart;
		subdomain(x, istart, ipoints);
	}
}

void sub_ser(float * x, size_t npoints)
{
	for (size_t i=0; i<npoints; i++)
		x[i] = rand();
}



int main(int argc, char** argv)
{
	// demo_2();
	// demo_sync_difficult();

	for (auto i=0; i<TRIALS; i++)
		sub_ser(ar.data(), ARRSZ);
}
