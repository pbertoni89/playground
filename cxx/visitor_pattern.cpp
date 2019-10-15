/**
 * https://sourcemaking.com/design_patterns/visitor/cpp/2
 */

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <memory>

using namespace std;

// #define VIRT virtual
#define VIRT

class Conn;
class Coherent;
class Visitor;

//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

std::vector<std::shared_ptr<Conn>> vspPersistent;

// if T == Coherent, error: no matching function for call to ‘std::shared_ptr<Coherent>::shared_ptr(std::shared_ptr<Conn>)’
std::vector<std::shared_ptr<Conn>> vspCoherent;

//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

class Conn : public enable_shared_from_this<Conn>
{
	virtual void _run() = 0;
public:
	void run();
	virtual void accept(class Visitor & v) = 0;
	std::shared_ptr<Conn> getptr()   { return shared_from_this(); }
};

class Ctrl: public Conn
{
	void _run() override;
public:
	VIRT void accept(Visitor & v) override;
	string foo_ctrl() { return "Ctrl"; }
};

class Coherent : public Conn {};

class Pc: public Coherent
{
	void _run() override;
public:
	VIRT void accept(Visitor & v) override;
	string foo_pc() { return "Pc"; }
};

class Pr: public Coherent
{
	void _run() override;
public:
	VIRT void accept(Visitor & v) override;
	string foo_pr() { return "Pr"; }
};

//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

class Visitor
{
public:
	virtual void visit(Ctrl & e) = 0;
	virtual void visit(Pc & e) = 0;
	virtual void visit(Pr & e) = 0;
};


// There must be empty, since every Conn is visited by every Visitor
void Ctrl::accept(Visitor & v)  { v.visit(*this); cerr << " Ctrl::accept "; }
void Pr::accept(Visitor & v)    { v.visit(*this); cerr << " Pr::accept "; }
void Pc::accept(Visitor & v)    { v.visit(*this); cerr << " Pc::accept "; }

void Conn::run() { cerr << "running "; _run(); }
void Pc::_run() { cerr << "_runPc\n"; }
void Pr::_run() { cerr << "_runPr\n"; }
void Ctrl::_run() { cerr << "_runCtrl\n"; }

//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

class VisitorPersistent: public Visitor
{
	VIRT void visit(Pc & e)   { cerr << "Persistent on " + e.foo_pc() << '\n'; }
	VIRT void visit(Pr & e)   { cerr << "Persistent on " + e.foo_pr() << '\n'; }
	VIRT void visit(Ctrl & e) {
		cerr << "Persistent on " + e.foo_ctrl() << " * * * ";
		vspPersistent.emplace_back(e.getptr());
		std::thread t(&Conn::run, & e);
		t.join();
	};
};

class VisitorCoherent: public Visitor
{
	VIRT void visit(Ctrl & e) { cerr << "Coherent on " + e.foo_ctrl() << '\n';  }
	VIRT void visit(Pc & e)   { cerr << "Coherent on " + e.foo_pc() << '\n'; }
	VIRT void visit(Pr & e)   {
		cerr << "Coherent on " + e.foo_pr() << " * * * ";
		vspCoherent.emplace_back(e.getptr());
		std::thread t(&Conn::run, & e);
		t.join();
	}
};

class VisitorThreadNo: public Visitor
{
	VIRT void visit(Ctrl & e) { cerr << "ThreadNo on " + e.foo_ctrl() << '\n';  }
	VIRT void visit(Pr & e)   { cerr << "ThreadNo on " + e.foo_pr() << '\n'; }
	VIRT void visit(Pc & e)   {
		cerr << "ThreadNo on " + e.foo_pc() << " * * *\n";
	}
};

//  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

int main()
{
	{
		std::vector<std::shared_ptr<Conn>> vspConns{ std::make_shared<Pc>(), std::make_shared<Pr>(), std::make_shared<Ctrl>() };

		cerr << "Conns: (Pc, Pr, Ctrl): ";
		for (const auto & sp : vspConns)
			cerr << sp << "\t";
		cerr << "\n";
		
		VisitorPersistent vp;
		VisitorCoherent vc;
		VisitorThreadNo vn;

		for (auto & pconn : vspConns)
			{ pconn->accept(vp); cerr << "\n#1\n"; }
		cerr << "\n";
		for (auto & pconn : vspConns)
			{ pconn->accept(vc); cerr << "\n#2\n"; }
		cerr << "\n";
		for (auto & pconn : vspConns)
			{ pconn->accept(vn); cerr << "\n#3\n"; }

		
		cerr << "\n\nCohe: ";
		for (const auto & sp : vspCoherent)
			cerr << sp << "\t";
		cerr << "\nPers: ";
		for (const auto & sp : vspPersistent)
			cerr << sp << "\t";
		cerr << "\n";
	}

	cerr << "Cohe: ";
	for (const auto & sp : vspCoherent)
		cerr << sp << "\t";
	cerr << "\nPers: ";
	for (const auto & sp : vspPersistent)
		cerr << sp << "\t";
	cerr << "\n";
}

