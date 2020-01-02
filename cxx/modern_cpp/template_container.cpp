#include <map>
#include <memory>
#include <string>
#include <iostream>

/**
 * Abstract placeholder for Cache polymorphism
 */
class ICache
{
public:

	virtual void update() {};

	friend std::ostream & operator << (std::ostream & out, const ICache & IC)
	{
		out << "you should never print this";
		return out;
	}
};

/**
 * Concrete. Coupling a name with some cached value
 */
template<typename T>
class Cache :
	public ICache
{
	const std::string m_name;

public:

		T m_cached;

	Cache(const std::string & name) :
		m_name(name),
		m_cached(0)
	{}

	void update() override
	{
		// m_cached is synced with remote resource; action depends both from T and m_name
	}

	void set(const T t)
	{
		std::cout << m_name << " setting " << +m_cached << " -> " << +t << std::endl;
		m_cached = t;
	}

	inline T get() const noexcept { return m_cached; }

	friend std::ostream & operator << (std::ostream & out, const Cache & O)
	{
		out << "Cache<" << O.m_name << ", " << O.m_cached << ">";
		return out;
	}
};

class CacheMap
{
	std::map<std::string, std::unique_ptr<ICache>> m_map;

	template<typename T>
	Cache<T>* _get_ptr(const std::string & name) const
	{
		return reinterpret_cast<Cache<T>*>(m_map.at(name).get());
	}

public:

	template<typename T>
	T get(const std::string & name) const
	{
		return _get_ptr<T>(name)->get();
	}

	template <typename T>
	void set(const std::string & name, T t)
	{
		_get_ptr<T>(name)->set(t);
	}

	template <typename T>
	void insert(const std::string & name, T def = 0)
	{
		std::unique_ptr<ICache> up = std::make_unique<Cache<T>>(name);
		m_map.insert({name, std::move(up)});
		set<T>(name, def);
	}

	friend std::ostream & operator << (std::ostream & out, const CacheMap & OM)
	{
		out << "OM{";
		for (const auto & IO : OM.m_map)
			out << IO.first << ": " << *(IO.second.get()) << ", ";    // ver1
			// out << IO.first << ": " << (IO.second->get()) << ", "; // ver2
			// out << IO.first << ": " << IO.second->m_cached << ", "; // ver3
		out << "}";
		return out;
	}
};


int main()
{
	CacheMap m;
	int i= 70000;

	m.insert<int>("i", 69999);
	m.insert<short>("s", 699);
	m.insert<char>("c", 69);
	m.set("i", i);

	std::cout << m << std::endl;
}
