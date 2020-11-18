# distutils: language=c++
from libcpp.algorithm cimport sort    # step 1
# from libcpp.algorithm cimport copy_n  # step 2
from libcpp.vector cimport vector     # step 3
# from libcpp.execution cimport par     # step 4


def cppsort(int[:] x) -> None:
    """
        Accepts a single parameter x.
        The preceding int[:] specifies the type of x, in this case, a one-dimensional array of integers.
        Calls std::sort to sort the elements of x.
        The arguments &x[0] and &x[-1] + 1 refer to the first and one past the last element of the array, respectively.
        More at https://cython.readthedocs.io/en/latest/src/tutorial/index.html
    """
    sort(&x[0], &x[-1] + 1)


"""
def cppsort(int[:] x):
    cdef vector[int] temp
    temp.resize(len(x))
    copy_n(&x[0], len(x), temp.begin())
    sort(par, temp.begin(), temp.end())
    copy_n(temp.begin(), len(x), &x[0])
"""


# first line tells Cython to use C++ mode as opposed to C (the default)
# keyword cimport is used to import C/C++ functionality.
# finally, recompile me with
#     conda activate ml36
#     cythonize -i cppsort.pyx
# we'll produce both a source file and a Python extension module (a .so file)
