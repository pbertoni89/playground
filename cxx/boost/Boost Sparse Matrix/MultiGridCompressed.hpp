#pragma once

#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
#include <array>
#include <vector>
#include <iomanip> // for print only
//#include <type_traits>

 #define UBLAS_COMPRESSED


/**
 * * Number of elements in a grid described by Ts dimensions
 * Recursive metafunction - exit step
 * @param x
 * @return 
 */
template<typename T>
constexpr T meta_prod(T x)
{
	return x;
}

/**
 * Number of elements in a grid described by Ts dimensions
 * Recursive metafunction - iteration step
 * @param x
 * @param xs
 * @return 
 */
template<typename T, typename... Ts>
constexpr T meta_prod(T x, Ts... xs)
{
	return x * meta_prod(xs...);
}


namespace detail
{
	template <typename Iter, size_t D0>
	size_t flatten(Iter first, Iter, std::index_sequence<D0> )
	{
		return *first;
	}

	template <typename Iter, size_t D0, size_t... DIMS>
	size_t flatten(Iter first, Iter last, std::index_sequence<D0, DIMS...> )
	{
		return *first * meta_prod(DIMS...) + 
			   flatten(std::next(first), last, std::index_sequence<DIMS...>{} );
	}
}    

/**
 * pretty-print for std::vector
 */
template<class T, size_t N> 
std::ostream & operator<<(std::ostream & os, const std::array<T, N> & V)
{
	if (V.size())
	{
		os << "{";
		for (auto && el : V)
		{
			os << el << "; ";
		}
		return os << "}";
	}
	else
	{
		return os << "{}";
	}
}


template<typename T, size_t... DIMS>
class MultiGridCompressed
{
public:
	static constexpr size_t ND = sizeof...(DIMS);
	static constexpr size_t NE = meta_prod(DIMS...);
	static_assert(ND > 0 and NE > 0, "MeshGrid invalid dimensions");

	/**
	 * Flattened array - the NE grid elements
	 */
	using ArrayValues =
#ifdef UBLAS_COMPRESSED
			boost::numeric::ublas::compressed_vector<T>;
#else
			std::vector<T>;
#endif

	/**
	 * Coordinates array of ND indexes. Dimension is contained,
	 * no particular reason to adopt a vector
	 */
	using ArrayCoord = std::array<size_t, ND>;

	/**
	 * Array of NE precomputed coordinates
	 */
	using MapIndexToCoord = std::vector<ArrayCoord>;

	using value_type      = typename ArrayValues::value_type;      // T
	using reference       = typename ArrayValues::reference;       // T&
	using const_reference = typename ArrayValues::const_reference; // const T&
	using size_type       = typename ArrayValues::size_type;       // size_t
	using iterator        = typename ArrayValues::iterator;        // random access iterator
	using const_iterator  = typename ArrayValues::const_iterator;

	iterator begin()
	{
		return m_values.begin();
	}
	const_iterator begin() const
	{
		return m_values.begin();
	}
	const_iterator cbegin() const
	{
		return m_values.cbegin();
	}
	iterator end()
	{
		return m_values.end();
	}
	const_iterator end() const
	{
		return m_values.end();
	}
	const_iterator cend() const
	{
		return m_values.cend();
	}

	reference operator[] (size_type idx)
	{
		return m_values[idx];
	};

	const_reference operator[] (size_type idx) const
	{
		return m_values[idx];
	};

	reference operator[] (const ArrayCoord & coord)
	{ 
		return m_values[detail::flatten(coord.begin(), coord.end(), std::index_sequence<DIMS...>{})];
	}

	const_reference operator[] (const ArrayCoord & coord) const
	{ 
		return const_cast<reference>(static_cast<const MultiGridCompressed &>(*this)[coord]); 
	};

	auto get_coord_from_index(size_type idx) const
	{
		return m_map_idx_to_coord.at(idx);
	}

	size_type get_index_from_coord(const ArrayCoord & coord) const
	{
		return detail::flatten(coord.begin(), coord.end(), std::index_sequence<DIMS...>{});
	}

	/**
	 * Unique, default constructor
	 */
	MultiGridCompressed() :
		m_map_idx_to_coord(fill_map_idx_to_coord()),
		m_values(fill_values())
	{}

private:

	auto fill_values() const
	{
		ArrayValues av(NE);
		return av;
	}


	auto fill_map_idx_to_coord() const
	{
		MapIndexToCoord coord(NE);

		std::array<size_t, ND> size_per_dim{{DIMS...}};

		for (size_t j = 0; j < NE; j ++)
		{
			// coord[j] = ArrayCoord(ND);

			size_t a = j, b = NE, r = 0;
			for (size_t i = 0; i <= ND - 2; i ++)
			{
				b /= size_per_dim[ND - i - 1];
				coord[j][ND-i-1] = a / b;
				r = a % b;
				a = r;
			}
			coord[j][0] = r;
		}
		return coord;
	}


	friend auto & operator<< (std::ostream & os, const MultiGridCompressed & MG)
	{
		static size_t count = 0;

		os << "MeshGrid of " << ND << " dimensions, " << NE << " elements.\n";
#ifdef UBLAS_COMPRESSED
		os << "Non-zeroes: " << MG.m_values.nnz() << '\n'
			<< "Allocated storage for " << MG.m_values.nnz_capacity() << '\n';
#endif

		os << "Values : {";
		for (auto&& v : MG.m_values)
		{
			os << v << "; ";
		}

		os << "}\nMap IndexToCoord :\n";
		for (auto&& m : MG.m_map_idx_to_coord)
		{
			os << std::setw(3) << (count++) << ":" << m;
			os << (count % 8 ? "\t" : "\n");
		}
		return os << "\n";
	}

	/**
	 * O(1) access flat index -> dim coordinates
	 */
	MapIndexToCoord m_map_idx_to_coord;

	/**
	 * same behaviour as declaring  `float values_[meta_prod(DIMS)];`
	 */
	ArrayValues m_values;
};


class Foo
{
	friend auto & operator<< (std::ostream & os, const Foo & F)
	{
		return os << "Foo{" << F.x << ", " << F.y << "}";
	}

public:
	int x;
	float y;

	Foo() : x(-123456789), y(3.14159) {}

	Foo(int _x, float _y) : x(_x), y(_y) {}
};
