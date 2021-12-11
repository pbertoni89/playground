#include <memory>
#include <functional>
#include <iostream>

#define DUCKTYPING
// #define OURS


#ifdef DUCKTYPING
	template<typename R, typename ... A>
	R test(std::function<R(A...)> f)
	{
		std::cout << "testing\n";
		return f(std::forward<A>(args)...);
	}

	template<typename R, typename ... A>
	void test(R f(A...))
	{
		test(std::function<R(A...)>(f));
	}
#else
	template<typename R, typename ... A>
	void test(R f(A...))
	{
		return f(std::forward<A>(args)...);
	}
#endif


int foo(int n)
{
	std::cout << "foo(" << n << ") -> " << (n*2) << std::endl;
	return n*2;
}


/* --*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-- */

struct Client
{};

struct Request
{};

struct Response
{};


Response _my_impl(Client & c, uint32_t & u, Request & r)
{
	std::cout << "_my_impl\n";
	return {};
}


auto up = std::make_unique<Client>();
uint32_t callback;

#ifdef OURS
template <typename Req, typename Resp, typename Fun, typename ... Args>
Response
_my_forward(std::function<Resp(Args ..., Req &)> fun, Args && ... args, Req & in)
{
	// return static_cast<Resp>Fun(*(up.get()), in);
	return fun(*(up.get()), callback, in);
}


Response my_api(Request & r)
{
	/* mle::tcp::ClientManagerMono::_my_forward<inference::Blob, mle::tcp::t_predict_response>(mle::tcp::t_predict_response (&)(xis::utils::tcp::Client&, uint32_t&, inference::Blob&), xis::utils::tcp::Client&, uint32_t&, inference::Blob&)  */
	// return _my_forward<inference::Blob, t_predict_response>(&IClientManager::_my_impl, *(m_upClient.get()), m_uCallbackId, blob);
	/* mle::tcp::ClientManagerMono::_my_forward(mle::tcp::t_predict_response (&)(xis::utils::tcp::Client&, uint32_t&, inference::Blob&), xis::utils::tcp::Client&, uint32_t&, inference::Blob&)
	return _my_forward(&IClientManager::_my_impl, *(m_upClient.get()), m_uCallbackId, blob);  */

	return _my_forward(&_my_impl, r);
}
#endif

int main()
{
	test(foo);

#ifdef OURS
	Request req;
	auto res = my_api(r);
#endif
	return 0;
}
