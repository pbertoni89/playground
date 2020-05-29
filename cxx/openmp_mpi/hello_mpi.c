/**
 * https://helloacm.com/simple-tutorial-with-openmp-how-to-use-parallel-block-in-cc-using-openmp/
 *
 * Build with
 * gcc -o hello_openmp hello_openmp.c -fopenmp
 */

#include <stdio.h>
#include <omp.h>
#include <mpi.h>


void demo_1()
{
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Get the name of the processor
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	// Print off a hello world message
	printf("Hello world from processor %s, rank %d out of %d processors\n",
			processor_name, world_rank, world_size);

	// Finalize the MPI environment
	MPI_Finalize();
}


void demo_2()
{
	int x = 1;
	int y = x + 2;
	int data;

	#pragma omp parallel num_threads(y * 3)
	{
		int id = omp_get_thread_num();
		data = id; // threads may interleaving the modification, because data scope is on master thread
		int total = omp_get_num_threads();
		printf("Greetings from process %d out of %d with Data %d\n", id, total, data);

		// make sure only 1 thread exectutes the critical section at a time
		#pragma omp critical
		{
			data = id; // threads may interleaving the modification
			printf("Critical from process %d out of %d with Data %d\n", id, total, data);
		}
	}

	printf("parallel for ends.\n");
}


int main(int argc, char** argv)
{
	demo_1();
	demo_2();
}
