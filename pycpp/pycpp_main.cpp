/*
 * SOURCE
 *	http://www.fuzihao.org/blog/2017/06/20/Run-Python-Code-in-C/
 * 
 * LINK
 * -lpython2.7 or -lpython3.5m
 */

/** test with 27 or 35											*/
#define PYTHON_VERSION 27

#if PYTHON_VERSION == 27
	#include <python2.7/Python.h>
#else
	#include <python3.5/Python.h>
	#include <python3.5m/Python.h>
#endif

#include <iostream>
#include <vector>
#include <random>

#include <iomanip>
#include <string>
// unnecessary
#include <map>
#include <cmath>

using t_v_d = std::vector<double>;


void print_dist_buckets(const t_v_d & dist)
{
	const auto DIV = dist.size() / 50;
	std::map<int, int> hist;

	for (const auto & d : dist)
	{
		hist[std::round(d)] ++;
	}

	for (const auto & p : hist)
	{
		std::cout << std::fixed << std::setprecision(1) << std::setw(2)
					<< p.first << ' ' << std::string(p.second/DIV, '*')
					<< std::endl;
	}
}


template<typename D>
t_v_d fill_dist(size_t sz, D & dist)
{
	t_v_d retval(sz);
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int n=0; n<sz; n++)
	{
		retval[n] = dist(gen);
	}
	return retval;
}

t_v_d example_normal_buckets(size_t sz = 10000, double mu = 0.0, double sigma = 1.0)
{
	std::normal_distribution<double> n(mu, sigma);
	auto dist = fill_dist(sz, n);
	print_dist_buckets(dist);
	return dist;
}


t_v_d example_uniform_buckets(size_t sz = 10000, double low = 0.0, double high = 10.0)
{
	std::uniform_real_distribution<double> u(low, high);

	auto dist = fill_dist(sz, u);
	print_dist_buckets(dist);
	return dist;
}


//   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

int main()
{
	// foo_example();
	example_normal_buckets();
	std::cout << " ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
	example_uniform_buckets();


	//  -  - CTOR  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	char PROGNAME[256] = "myPythonKsTestProgram";

	Py_SetProgramName(PROGNAME);
	Py_Initialize();

	// run python code to load functions
	PyRun_SimpleString("exec(open('ks_test.py').read())");
	// get __main__ module
	PyObject * pPyMainModule = PyImport_AddModule("__main__");
	PyObject * pPyGlobalDict = PyModule_GetDict(pPyMainModule);
	// get ksverdict function
	PyObject * pPyFunc = PyDict_GetItemString(pPyGlobalDict, "ksverdict");

	//  -  -  RUNTIME  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

	// par is a parameter to send to python function
	PyObject * pPyPar;
/*
	par = PyDict_New();
	PyDict_SetItemString(par, "x0", PyFloat_FromDouble(1.0));
	PyDict_SetItemString(par, "x1", PyFloat_FromDouble(2.0));
	PyObject * par = PyTuple_Pack(1, par);
*/

	// Cast std::vector<double> to PyList
	auto vDist = example_normal_buckets(10000, 2.0, 10.0);
	// auto vDist = example_uniform_buckets(10000, 2.0, 10.0);	

	pPyPar = PyList_New(vDist.size());
	for (int i=0; i<vDist.size(); i++)
	{
		PyList_SetItem(pPyPar, i, PyFloat_FromDouble(vDist[i]));	
	}

	// parameter should be a tuple
	PyObject * pPyParTuple = PyTuple_Pack(1, pPyPar);
	Py_DecRef(pPyPar);

	// call the function
	PyObject * pPyRetval = PyObject_CallObject(pPyFunc, pPyParTuple);
	Py_DecRef(pPyParTuple);

	int retval = -1;

	if (pPyRetval != nullptr)
	{
		// PyInt_AsLong is deprecated in Python 3.5m
#if PYTHON_VERSION == 27
		if (PyInt_Check(pPyRetval))
		{
			retval = PyInt_AsLong(pPyRetval);
		}
#else
		if (PyLong_Check(pPyRetval))
		{
			retval = PyLong_AsLong(pPyRetval);
		}
#endif
		else
		{
			std::cerr << "Unexpected result\n";
		}
		Py_DecRef(pPyRetval);
		std::cout << "Return of call : " << retval << std::endl;
	}

	/*
	 * Other Examples
	 *
	 * pure python code
	 * PyRun_SimpleString("print ('Hello World.')");
	 *
	 * a call with no parameters
	 * PyObject_CallObject(PyDict_GetItemString(global_dict, "foo_kstest"), nullptr);
	 */

	//  -  - DTOR  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	Py_DecRef(pPyFunc);
	Py_Finalize();
}
