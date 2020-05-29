#include <iostream>
#include <omp.h>

constexpr size_t NTHREADS_MAX = 24;
constexpr long NSTEPS_MAX = 1'000'000'000;


/**
 * 	 1 threads --> 0.57 seconds
 * 	 4 threads --> 1.34 seconds
 * 	24 threads --> 0.59 seconds
 * 	48 threads --> 0.46 seconds
 */
void integral_serial(size_t ns)
{
	double pi = 0.0;
	double sum = 0.0;

	const double step = 1.0 / (double) ns;
	const double timer_start = omp_get_wtime();

	for (int i = 0; i < ns; i++)
	{
		const double x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}

	pi = sum * step;

	const double timer_took = omp_get_wtime() - timer_start;
	std::cout << "integral_serial: " << pi << " took " << timer_took << "\n";
}


/**
 * 	 1 threads --> 0.57 seconds
 * 	 4 threads --> 1.34 seconds
 * 	24 threads --> 0.59 seconds
 * 	48 threads --> 0.46 seconds
 */
void integral_roundrobin(size_t ns, size_t nt)
{
	int nthreads;
	double pi = 0.0;
	double sum[NTHREADS_MAX];

	const double step = 1.0 / (double) ns;
	const double timer_start = omp_get_wtime();
	omp_set_num_threads(nt);

	#pragma omp parallel
	{
		int i, id, lnthreads;
		double x;

		lnthreads = omp_get_num_threads();
		id = omp_get_thread_num();
		if (id == 0)
			nthreads = lnthreads;

		for (i = id, sum[id]=0; i < ns; i+=lnthreads)
		{
			x = (i + 0.5) * step;
			sum[id] += 4.0 / (1.0 + x * x);
		}

	}

	for (int i = 0; i < nthreads; ++i)
		pi += sum[i] * step;

	const double timer_took = omp_get_wtime() - timer_start;
	std::cout << "integral_roundrobin: " << pi << " took " << timer_took << "\n";
}


/**
 * 	 1 threads --> 0.53 seconds
 * 	 4 threads --> 1.34 seconds
 * 	24 threads --> 0.59 seconds
 * 	48 threads --> 0.46 seconds
 */
void integral_atomic(size_t ns, size_t nt)
{
	double pi = 0.0;

	const double step = 1.0 / (double) ns;
	omp_set_num_threads(nt);
	const double timer_start = omp_get_wtime();

	#pragma omp parallel
	{
		int i, id, lnthreads;
		double x, sum = 0;

		lnthreads = omp_get_num_threads();
		id = omp_get_thread_num();

		for (i = id; i < ns; i+=lnthreads)
		{
			x = (i + 0.5) * step;
			sum += 4.0 / (1.0 + x * x);
		}

		#pragma omp atomic
		pi += sum * step;

	}

	const double timer_took = omp_get_wtime() - timer_start;
	std::cout << "integral_atomic: " << pi << " took " << timer_took << "\n";
}


/**
 * 	 1 threads --> 0.55 seconds
 * 	 4 threads --> 0.24 seconds
 * 	24 threads --> 0.24 seconds
 * 	48 threads --> 0.23 seconds
 */
void integral_reduction(size_t ns, size_t nt)
{
	double pi = 0.0;
	double sum = 0;
	int i = 0;

	const double step = 1.0 / (double) ns;
	omp_set_num_threads(nt);
	const double timer_start = omp_get_wtime();

	#pragma omp parallel for reduction(+:sum)
	for (i = 0; i < ns; ++i)
	{
		int x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}

	pi = sum * step;
	const double timer_took = omp_get_wtime() - timer_start;
	std::cout << "integral_reduction: " << pi << " took " << timer_took << "\n";
}


/**
 * this version is better than `integral_reduction` because it can work in the case of non-threaded environments
 *
 * 	 1 threads --> 0.55 seconds
 * 	 4 threads --> 0.24 seconds
 * 	24 threads --> 0.24 seconds
 * 	48 threads --> 0.23 seconds
 */
void integral_better_reduction(size_t ns, size_t nt)
{
	double pi = 0.0;
	double sum = 0;
	int i = 0, x;

	const double step = 1.0 / (double) ns;
	omp_set_num_threads(nt);
	double timer_start = omp_get_wtime();

	#pragma omp parallel for private(x) reduction(+:sum)
	for (i = 0; i < ns; ++i)
	{
		x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}

	pi = sum * step;
	const double timer_took = omp_get_wtime() - timer_start;
	std::cout << "integral_better_reduction: " << pi << " took " << timer_took << "\n";
}


int main()
{
	std::cout << "integral with " << NTHREADS_MAX << " threads, " << NSTEPS_MAX << " steps\n";

	for (size_t ns=100'000; ns<NSTEPS_MAX; ns*=10)
	{
		std::cout << "* * * steps " << ns << "\n";
		integral_serial(ns);
		for (size_t nt=1; nt<NTHREADS_MAX; nt*=2)
		{
			std::cout << "* * * threads " << nt << "\n"; 
			integral_roundrobin(ns, nt);
			integral_atomic(ns, nt);
			integral_reduction(ns, nt);
			integral_better_reduction(ns, nt);
		}
	}
}
