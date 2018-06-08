/**
 * https://codereview.stackexchange.com/questions/97260/generic-multi-dimension-grid-array-class-in-c
 *	->
 * http://coliru.stacked-crooked.com/a/96cf1e8570143d6f
 */

#include <iostream>
#include <array>
#include <vector>
#include <type_traits>

#include <algorithm> // just for std::generate in main()


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

// pretty-print for std::array
template<class T, size_t N> 
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr)
{
	os << "{";
	for (auto && el : arr)
	{
		os << el << ";";
	}
	return os << "}";
}


template<typename T, size_t... DIMS>
class MultiGrid
{
public:
	static constexpr size_t ND = sizeof...(DIMS);
	static constexpr size_t NE = meta_prod(DIMS...);
	static_assert(ND > 0 and NE > 0, "Grid dimensions problem...");

	/**
	 * Flattened array - the NE grid elements
	 */
	using ArrayValues     = std::array<T, NE>;

	/**
	 * Coordinates array of ND indexes
	 */
	using ArrayCoord      = std::array<size_t, ND>;

	/**
	 * N
	 */
	using MapIndexToCoord = std::array<ArrayCoord, NE>;

	using value_type      = typename ArrayValues::value_type;      // T
	using reference       = typename ArrayValues::reference;       // T&
	using const_reference = typename ArrayValues::const_reference; // const T&
	using size_type       = typename ArrayValues::size_type;       // size_t
	using iterator        = typename ArrayValues::iterator;        // random access iterator
	using const_iterator  = typename ArrayValues::const_iterator;

	/**
	 * default constructor use delegating constructor
	 */
	MultiGrid() :
		MultiGrid(ArrayValues{})
	{}

	MultiGrid(const ArrayValues & values) :
		m_map_idx_to_coord(fill_map_idx_to_coord()),
		m_values(values)
	{}

	iterator       begin()        { return m_values.begin();  }
	const_iterator begin()  const { return m_values.begin();  }
	const_iterator cbegin() const { return m_values.cbegin(); }
	iterator       end()          { return m_values.end();    }
	const_iterator end()    const { return m_values.end();    }
	const_iterator cend()   const { return m_values.cend();   }

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
		return const_cast<reference>(static_cast<const MultiGrid &>(*this)[coord]); 
	};

	auto get_coord_from_index(size_type idx) const
	{
		return m_map_idx_to_coord.at(idx);
	}

	size_type get_index_from_coord(const ArrayCoord & coord) const
	{
		return detail::flatten(
			coord.begin(), coord.end(), std::index_sequence<DIMS...>{});
	}

private:

	auto fill_map_idx_to_coord() const
	{
		MapIndexToCoord coord;
		std::array<size_t, ND> size_per_dim{{DIMS...}};

		for (size_t j = 0; j < NE; j ++)
		{
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

	friend auto & operator<< (std::ostream & os, const MultiGrid & MG)
	{
		os << "Values : {";
		for (auto&& v : MG.m_values)
		{
			os << v << ";";
		}
		os << "}\nMapping index to coord :\n";
		size_t count{0};
		for (auto&& m : MG.m_map_idx_to_coord)
		{
			os << count ++ << ":" << m << "\t";
		}
		return os << "\n";
	}

	MapIndexToCoord m_map_idx_to_coord;    // O(1) access flat index -> dim coordinates
	ArrayValues     m_values;              // same behaviour as declaring  `float values_[meta_prod(DIMS)];`
};


void example_multi_dimension_grid()
{
	// Create a 4D grid with 3x2x3x5 vertices
	MultiGrid<float, 3, 2, 3, 5> grid;

	// grid behaves like a STL container and we can fill values with std::generate
	std::generate(grid.begin(), grid.end(), []()
		{
		static float n{0.0f}; return n+=0.5f;
		});

	std::cout << grid << std::endl;

	// get coordinates from index
	std::cout << "get_coord_from_index(43) = " << grid.get_coord_from_index(43) << std::endl;
	// and vice versa
	std::cout << "get_index_from_coord({{2,0,2,3}}) = " << grid.get_index_from_coord({{2,0,2,3}}) << std::endl;
	// print value at specific coordinates
	std::cout << "Grid[{{2,0,2,3}}] = " << grid[{{2,0,2,3}}] << std::endl;
	// print value at specific index
	std::cout << "Grid[42] = " << grid[42] << "\n\n";

	MultiGrid<float, 2, 2> little_grid;
	std::cout << little_grid << std::endl;

	std::cout << "sizeof(MultiGrid<5,int,2,2,2,2,2>) = " << sizeof(MultiGrid<int, 2, 2, 2, 2, 2>) << std::endl;
	std::cout << "bye bye" << std::endl;
}
