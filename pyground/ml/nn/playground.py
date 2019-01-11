# -*- coding: utf-8 -*-
from __future__ import print_function
from functools import partial
import pathos.multiprocessing as mp
import multiprocessing
import tensorflow as tf
import numpy as np


#
#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .
# https://stackoverflow.com/questions/26059764/python-multiprocessing-with-pathos
#

class Bar:
    def __init__(self):
        self.sum = 0
        self.calls = 0

    def foo(self, name):
        self.calls += 1
        # print("foo call {}".format(name))
        return len(str(name))

    def boo(self, things):
        for thing in things:
            self.sum += self.foo(thing)
        return self.sum


class Manager:
    def __init__(self):
        self.b = Bar()

    def test(self):
        results = mp.ProcessingPool().map(self.b.boo, [[12, 3, 456], [8, 9, 10], ['a', 'b', 'cde']])
        print(str(results))
        print("sum is {}, calls {}".format(self.b.sum, self.b.calls))
        return results


def demo_pathos():
    m = Manager()
    m.test()


"""
    However, you can also see that when you look at b.sum, it's mysteriously still 0. Why is b.sum still zero?
    Well, what multiprocessing (and thus also pathos.multiprocessing) does, is make a COPY of whatever you pass through
    the map to the other python process... and then the copied instance is then called (in parallel) and return whatever
    results are called by the method invoked.

    Note you have to RETURN results, or print them, or log them, or send them to a file, or otherwise.
    They can't go back to the original instance as you might expect, because it's not the original instance that's sent
    over to the other processors. The copies of the instance are created, then disposed of -- each of them had their
    sum attribute increased, but the original `b.sum' is untouched.

    There is however, plans within pathos to make something like the above work as you might expect -- where the
    original object IS updated, but it doesn't work like that yet.
"""

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


def demo_tf_with_device():
    """
        https://www.tensorflow.org/guide/using_gpu
    """
    c = None
    with tf.device('/GPU:0'):
        dim_eqs, dim_vars = 2000, 3000
        dim_mat, dim_runs = dim_eqs * dim_vars, 10
        for i in range(0, dim_runs):
            a = tf.constant(np.random.rand(dim_mat, 1), shape=[dim_eqs, dim_vars], name='a')
            b = tf.constant(np.random.rand(dim_mat, 1), shape=[dim_vars, dim_eqs], name='b')
            c = tf.matmul(a, b)
            print("\r{} %".format(i * 100.0 / dim_runs), end="")

    sess = tf.Session(config=tf.ConfigProto(allow_soft_placement=True, log_device_placement=True))
    print(sess.run(c))

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


def __fun_foo(x, r=1):
    """
        A quite foo function
        :param x: a 128*128 ndarray
        :param r: example additional parameter
        :return: x itself
    """
    print("my_function(r={}) is\n{}".format(r, np.array2string(x)))
    z = np.arange(10).reshape((2, 5))
    z += 1
    return x


def demo_multithread():
    print("__.__.__.__.__.__.__.__.__ DEMO START __.__.__.__.__.__.__.__.__")
    d1, d2, d3 = 2, 128, 128
    xx = np.arange(d1 * d2 * d3).reshape((d1, d2, d3))
    pool = multiprocessing.Pool(processes=d1)
    l_results = pool.map(__fun_foo, map(lambda x: x, xx))
    print("A got {} results".format(len(l_results)))
    l_results = pool.map(partial(__fun_foo, r=5), xx)
    print("B got {} results".format(len(l_results)))
    pool.close()
    pool.join()
    print("__.__.__.__.__.__.__.__.__ DEMO END __.__.__.__.__.__.__.__.__")


def demo_lambda():
    a = [1, 2, 3, 4, 5]
    b = [2, 2, 9, 0, 9]
    c = map(lambda my_pair: max(my_pair), zip(a, b))
    d = map(lambda my_pair: my_pair, zip(a, b))
    return c, d

#  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .  ~  .


if __name__ == "__main__":
    demo_pathos()
    demo_tf_with_device()
    demo_multithread()
    demo_lambda()
