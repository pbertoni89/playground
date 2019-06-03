#include <functional>
#include <map>
#include <iostream>
#include <string>


/**
	* http://simmesimme.github.io/tutorials/2015/09/20/signal-slot
	*
	* A signal object may call multiple slots with the
	* same signature. You can connect functions to the signal
	* which will be called when the emit() method on the
	* signal object is invoked. Any argument passed to emit()
	* will be passed to the given functions.
	*/
template <typename... Args>
class Signal
{

	mutable std::map<int, std::function<void(Args...)>> m_slots;

	mutable int m_current_id;

public:

	Signal()
		: m_current_id(0)
	{}

	/**
	* copy creates new signal
	*/
	Signal(const Signal & other)
		: m_current_id(0)
	{}

	/**
	* assignment creates new Signal
	*/
	Signal& operator=(const Signal & other)
	{
		disconnect_all();
	}

	/**
	* connects a member function to this Signal
	*/
	template <typename T>
	int connect_member(T * inst, void (T::*func)(Args...))
	{
		return connect([=](Args... args)
		{ 
			(inst->*func)(args...); 
		});
	}

	/**
	* connects a const member function to this Signal
	*/
	template <typename T>
	int connect_member(T * inst, void (T::*func)(Args...) const)
	{
		return connect([=](Args... args)
		{
			(inst->*func)(args...); 
		});
	}

	/**
	* connects a std::function to the signal. The returned
	* value can be used to disconnect the function again
	*/
	int connect(const std::function<void(Args...)> & slot) const
	{
		m_slots.insert(std::make_pair(++m_current_id, slot));
		return m_current_id;
	}

	/**
	* disconnects a previously connected function
	*/
	void disconnect(int id) const
	{
		m_slots.erase(id);
	}

	/**
	* disconnects all previously connected functions
	*/
	void disconnect_all() const
	{
		m_slots.clear();
	}

	/**
	* calls all connected functions
	*/
	void emit(Args... p)
	{
		for(auto it : m_slots)
		{
			it.second(p...);
		}
	}
};


class Person
{
	std::string m_name;

public:
	Person(const std::string & name) :
		m_name(name)
	{}

	Signal<const std::string &> say;

	void listen(const std::string & message)
	{
		std::cout << m_name << " received: " << message << std::endl;
	}
};


int main(int argc, char ** argv)
{
	Person alice("Alice"), bob("Bob");

	alice.say.connect_member(&bob, &Person::listen);
	bob.say.connect_member(&alice, &Person::listen);

	alice.say.emit("Have a nice day!");
	bob.say.emit("Thank you!");
	
	return 0;
}
