// https://codereview.stackexchange.com/questions/201106/

// #include <cstddef>
#include <tuple>
// #include <type_traits>


namespace impl
{
	template<typename T, typename... Ts>
	struct First { using Type = T; };

	template<typename ... Ts>
	using First_t = typename First<Ts ...>::Type;

	template<auto value, auto ... values>
	inline constexpr auto all_same = (... && (value == values));

	template<class Tuple>
	inline constexpr auto tuple_size = std::tuple_size_v<std::remove_reference_t<Tuple>>;

	template<std::size_t index = 0, class Function, class... Tuples>
	constexpr void for_each_in_tuples(Function func, Tuples&&... tuples)
	{
		constexpr auto size = tuple_size<First_t<Tuples ...>>;

		func(std::get<index>(std::forward<Tuples>(tuples)) ...);
		if constexpr (index + 1 < size)
			for_each_in_tuples<index + 1>(func, std::forward<Tuples>(tuples) ...);
	}
}


template<class Function, class... Tuples>
constexpr void for_each_in_tuples(Function func, Tuples && ... tuples)
{
    static_assert(sizeof ...(Tuples) > 0);
    static_assert(impl::all_same<impl::tuple_size<Tuples>...>);

    impl::for_each_in_tuples(func, std::forward<Tuples>(tuples)...);
}


#include <iostream>
#include <tuple>


using indexes_t = std::tuple<int, unsigned int>;

[[nodiscard]] inline constexpr decltype(auto) belt_id(const indexes_t & indexes) { return std::get<0>(indexes); }
[[nodiscard]] inline constexpr decltype(auto) cam_id (const indexes_t & indexes) { return std::get<1>(indexes); }

[[nodiscard]] inline constexpr decltype(auto) belt_id(indexes_t & indexes) { return std::get<0>(indexes); }
[[nodiscard]] inline constexpr decltype(auto) belt_id(indexes_t && indexes) { return std::get<0>(indexes); }

[[nodiscard]] inline constexpr decltype(auto) cam_id (indexes_t && indexes) { return std::get<1>(indexes); }



int main()
{
	std::tuple t1(1, 2.2, false);
	std::tuple t2(3.3, 'a', 888);

	std::cout << std::boolalpha;
	for_each_in_tuples(
		[](auto a1, auto a2)
		{
			std::cout << a1 << ' ' << a2 << '\n';
		},
	t1, t2);

	// Outputs:
	// 1 3.3
	// 2.2 a
	// false 888
	
	using t_ii = std::tuple<int, int>;
	t_ii ii = {};
	std::cout << '\n' << std::get<0>(ii) << ',' << std::get<1>(ii) << '\n';
	int i = {};
	std::cout << i << '\n';

	const indexes_t idx1 = {-1, 1}, idx2 = std::make_tuple(-2, 2);
	indexes_t idx3 = {-10, 10}, idx4 = std::make_tuple(-2, 20);

	belt_id({-10, 10}) = -3;  // USELESS... COSA ME NE FREGA
	// belt_id(idx1) = -3;  // won't work ofc
	belt_id(idx3) = -66666;
	std::cout << "idx3: " << belt_id(idx3) << '\n';
	const auto e1 = (idx1 == idx2), e2 = (idx1 == std::make_tuple(-1, 1)); // inconsistent type deduction , e3 = (idx1 == {-1, 1});
	std::cout << "equals " << e1 << ',' << e2 << '\n';
}
