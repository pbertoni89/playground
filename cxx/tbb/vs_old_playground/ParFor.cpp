#include "ParFor.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <fstream>


void example_parallel_for_explore()
{
	srand(time(NULL));

	// path of project
	std::ofstream CSV("../../../parfor.csv");
	CSV << "Dim,Grain,Tp,Ts\n";

	const size_t LOG_DIM_MIN = 16, LOG_DIM_MAX = 32;
	const size_t LOG_GRAIN_MIN = 5, LOG_GRAIN_MAX = 31;
	const size_t N_SPACE = (LOG_DIM_MAX-LOG_DIM_MIN)*(LOG_GRAIN_MAX-LOG_GRAIN_MIN);
	size_t iSpace = 0;

	for (int iDim=LOG_DIM_MIN; iDim<LOG_DIM_MAX; iDim++)
	{
		for (int iGrain=LOG_GRAIN_MIN; iGrain<LOG_GRAIN_MAX; iGrain++)
		{
			const size_t DIM = std::pow(2, iDim), GRAIN = std::pow(2, iGrain);
			const auto rv = example_parallel_for(DIM, GRAIN);

			CSV << DIM << "," << GRAIN << "," << std::scientific << std::setprecision(2) << rv.time_par << "," << rv.time_ser << std::fixed << std::endl;
			
			const size_t SPACE_CURR = (++iSpace * 100);
			if (SPACE_CURR % N_SPACE == 0)
			{
				std::cout << (SPACE_CURR / N_SPACE) << "% ";
			}
		}
	}
}


t_perf example_parallel_for(size_t DIM, size_t GRAINSIZE)
{
	t_perf rv;

	std::vector<float> v_test(DIM), v_result_parallel(DIM), v_result_serial(DIM);
	// fill random values
	for (auto it = v_test.begin(); it != v_test.end(); it++)
	{
		*it = rand() % 65535;
	}

	// init task manager: tictoc is constant ~e-08
	ParForLogTask pp(v_test, v_result_parallel);
									const auto t0 = tic();
	// spawn par for
	tbb::parallel_for(tbb::blocked_range<size_t>(0, v_test.size(), GRAINSIZE), pp, tbb::simple_partitioner());
									const auto t1 = tic();
	// spawn serial for
	for (int i=0; i<v_test.size(); i++)
	{
		v_result_serial[i] = std::log2(v_test[i]);
	}
									const auto t2 = tic();
	// check results
#ifdef _DEBUG
	for (int i=0; i<v_test.size(); i++)
	{
		if (v_result_serial[i] != pp.m_out[i])
		{
			std::cerr << "mismatch[" << i << "]: " << v_result_serial[i] << " != " << pp.m_out[i] << "\n";
			throw std::runtime_error("value error");
		}
	}
#endif

	rv.time_par = tictoc(t0, t1);
	rv.time_ser = tictoc(t1, t2);
	return rv;
}


ParForLogTask::ParForLogTask(const std::vector<float> & V_IN, std::vector<float> & v_out) :
	m_in(V_IN),
	m_out(v_out)
{}


void ParForLogTask::operator()(const tbb::blocked_range<size_t> & BR) const
{
	const bool DUMP_OUTPUT =
#if _DEBUG
		(m_in.size() < 20);
#else
		false;
#endif
	std::stringstream ss;

	for (auto i = BR.begin(); i != BR.end(); ++i)
	{
		m_out[i] = _OP(m_in[i]);

		if (DUMP_OUTPUT)
		{
			ss << i << " log2(" << m_in[i] << ") = " << m_out[i] << std::endl;
		}
	}

	std::cerr << ss.str();
}
