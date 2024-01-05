/**
 * Sources
 * https://en.cppreference.com/w/cpp/thread/future
 * https://en.cppreference.com/w/cpp/thread/async
 */

#include<iostream>
#include<vector>
#include<future>
#include<thread>
#include<chrono>

class A
{
public:
	virtual int run() = 0;
};

class Pc : public A
{
	int m_id;
	
public:
	Pc(int id) : m_id(id)
	{}

	int run(/*float f*/) override
	{
		std::cerr << "pc_" << m_id << "_run_" << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(m_id));
		std::cerr << "pc_" << m_id << "_don\n";
		return m_id;
	}
};


void demo_cast_foo(std::shared_ptr<A> spa)
{
	std::shared_ptr<Pc> spc;
	spc = spa;
	std::cerr << reinterpret_cast<std::shared_ptr<Pc>>(spa)->m_id;
}

int demo_cast()
{
	std::shared_ptr<Pc> spc = std::make_shared<Pc>(34);
	demo_cast_foo(spc);
}


int main (int argc, char **argv)
{
	demo_cast();

	/* lo scope del main qui e' come quello della connessione Controller qui. tecnicamente non ce' piu nemmeno bisogno
	 * che il Controller sia una connessione. potrebbe essere un semplice "thread persistente" nel datasource. Anzi,
	 * tecnicamente gia' lo e', poiche' solamente le ConnectionCoherent finiscono gestite dal ConnectionManager, che,
	 * a questo punto, potrebbe rinominarsi CoherencyManager
	 *
	 * Le ConnectionProducerController dunque non avrebbero piu' bisogno di avviare un thread in CoherencyManager.
	 * Mantengono si' un loro "run" che idealmente ha come parametro il t_send_command condiviso, "sc".
	 * Se questo parametro non rompe il pippone di eredita' dei metodi virtuali (ora vediamo)
	 */
	std::vector<Pc> vpc;
	for (int i = 3; i < 13; i += 3)
		vpc.emplace_back(i);

	while (true)
	{
		// scarica una richiesta inviata dal WsController . . . . . bloccante
		// a questo punto ho un oggetto t_send_command
		std::vector<std::future<int>> va;

		// per ogni ConnectionProducerController, chiama in modo asincrono la sua run (passagli sc)
		// no non posso passare un corno a run, se questo non ha parametri!!! usero' un oggetto globale
		// o ancora meglio un membro di CoherencyManager tipo m_scCurrent
		for (auto & pc : vpc)
		{
			va.push_back( std::async(std::launch::async, [&pc] () { return pc.run(); }));
		}

		for (auto & a : va)
		{
			std::cerr << "main_" << a.get() << "\n";  // no need to wait if getting
		}
	}
}
