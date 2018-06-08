/**
 * Important: differences in sparse vector/matrix:
 *	- mapped
 *  - compressed
 *  - coordinate
 * http://boost.2283326.n4.nabble.com/Difference-between-the-different-sparse-matrix-types-td2710523.html
 *
 * http://www.guwi17.de/ublas/matrix_sparse_usage.html
 */

#include "MultiGrid.hpp"
#include "MultiGridCompressed.hpp"

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "boost/multi_array.hpp"

#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cassert>


using namespace boost::numeric::ublas;


void example_multi_array()
{
	// Create a 3D array that is 3 x 4 x 2
	using t_mat2 = boost::multi_array<double, 3>;
	int dim1 = 3, dim2 = rand() % 4 + 1, dim3 = 2;
	using t_mat2_i = t_mat2::index;
	t_mat2 A(boost::extents[dim1][dim2][dim3]);

	// Assign values to the elements
	int values = 0;
	for(t_mat2_i i = 0; i != dim1; ++i) 
		for(t_mat2_i j = 0; j != dim2; ++j)
			for(t_mat2_i k = 0; k != dim3; ++k)
				A[i][j][k] = values++;

	// Verify values
	int verify = 0;
	for(t_mat2_i i = 0; i != dim1; ++i) 
		for(t_mat2_i j = 0; j != dim2; ++j)
			for(t_mat2_i k = 0; k != dim3; ++k)
				assert(A[i][j][k] == verify++);

	std::cout << "test passed for multiarray(" << dim1 << ", " << dim2 << ", " << dim3 << ")" << std::endl;
}


typedef struct t_elem
{
	uint8_t r;
	float z;
	uint8_t label;
	
	t_elem() :
		r(rand() % 255), z(-1.0), label(0)
	{}

	t_elem(uint8_t _r, float _z, uint8_t _label) :
		r(_r), z(_z), label(_label)
	{}

	friend std::ostream & operator << (std::ostream & out, const t_elem & E)
	{
		// atoi prevents erroneus char reading
		// https://stackoverflow.com/questions/14644716/how-to-output-a-character-as-an-integer-through-cout
		out << "E{" << +E.r << ", " << E.z << ", " << +E.label << "}";
	}
} t_elem;


/**
 * https://stackoverflow.com/questions/15488588/c-use-boost-to-build-and-access-values-in-a-sparse-matrix
 */
void example_compressed()
{
	compressed_matrix<int> m(10, 10); // 10x10 compressed matrix

	// replace by ifstream in("filename") in real code
	std::istringstream in("1      1         4\n"
						  "1      3         5\n"
						  "2      1         6\n");

	// read from stream
	int val;
	for(size_t r,c; in >> r >> c >> val; )
	{
		m(r,c) = val;
	}

	// print out
	for(size_t i = 0; i < m.size1(); ++i)
	{
		for(size_t j = 0; j < m.size2(); ++j)
		{
			std::cout << m(i,j) << ' ';
		}
		std::cout << '\n';
	}

	// info on the storage
	std::cout << "Non-zeroes: " << m.nnz() << '\n'
			  << "Allocated storage for " << m.nnz_capacity() << "\n\n";
}


void example_mapped()
{
	const size_t N_ROWS = 3, N_COLS = 3;
	const size_t N_ELEMS = N_ROWS * N_COLS, NON_ZEROS = N_ELEMS - 6;

	mapped_matrix<t_elem*> m(N_ROWS, N_COLS, NON_ZEROS);

	size_t i, j;

	for (i = 0; i < m.size1(); ++i)
	{
		for (j = 0; j < m.size2(); ++j)
		{
			// alloca solo posizioni pari
			if ((i+j) % 2 == 0)
			{
				const double VAL = 3 * i + j;
				m(i, j) = new t_elem(rand() % 255, VAL, 0);
			}
			std::cout << "m(" << i << ", " << j << ") " << m(i, j) << std::endl;
		}
	}

	std::cout << "\nm " << m << std::endl;
	std::cout << "expected size " << (NON_ZEROS * sizeof(t_elem)) << ", size " << sizeof(m) << std::endl;
	std::cout << "Non-zeroes: " << m.nnz() << ", allocated storage for " << m.nnz_capacity() << "\n\n";
}


void example_multi_dimension_vector()
{
	std::cout << "~\t~\t~\t~\t~\t~\t~\tMESH_GRID<4, 4>\n";

	// const size_t RESOLUTION = random_resolution();
	const size_t RESOLUTION = 4;
	// const size_t DIMENSIONS = 2;

	MultiGridCompressed<float, RESOLUTION, RESOLUTION, RESOLUTION> mg1;

#ifdef UBLAS_COMPRESSED
	mg1[0] = 34.1;
	mg1[{{2, 3}}] = 53.1;
#else
	std::generate(mg1.begin(), mg1.end(), []()
		{
			static float n {0.0f};
			return n += 0.5f;
		});
#endif
	std::cout << "<<<<<mg1>>>>\n" << mg1 << std::endl;
	std::cout << "mg1[{{1, 3}}] should not exist: " << mg1[{{1, 3}}] << std::endl;



	Foo f1, f2(3, 4.5);

	MultiGridCompressed<Foo, RESOLUTION, RESOLUTION> mg2;

	mg2[0] = f1;
	mg2[1] = Foo(4, 6.9);
	mg2[2] = f2;

	std::cout << "<<<<<mg2>>>>\n" << mg2 << std::endl;
	std::cout << "mg2[{{1, 3}}] = mg2[" << mg2.get_index_from_coord({{1, 3}}) << "] should not exist: " << mg2[{{1, 3}}] << std::endl;
}


int main ()
{
	srand(time(NULL));

	example_multi_array();

	example_compressed();
	
	example_mapped();

	// example_multi_dimension_grid();

	example_multi_dimension_vector();
}