import time
import random
import numpy as np
# noinspection PyUnresolvedReferences
import cppsort

dt = np.int32


def demo_usage(rnd_max: int, rnd_n: int, verbose=False) -> float:
    """
        :param rnd_max:
        :param rnd_n: meaningful values are 2 << 22
        :param verbose:
        :return:
    """
    if verbose:
        print('demo running with [0, {}), N={}'.format(rnd_max, rnd_n))
    t0 = time.time()
    ll = [random.randint(0, rnd_max) for _ in range(int(rnd_n))]
    aa = np.array(ll, dtype=dt)  # data is copied by value
    # y = np.array(ll, dtype=dt)
    print(aa[:5])
    t1 = time.time()
    cppsort.cppsort(aa)  # np.sort(aa) takes quite the same time
    t2 = time.time()
    print(aa[:5])
    print(ll[:5])
    ll.sort()
    print(ll[:5])
    t3 = time.time()
    if verbose:
        print('init    took {:.6f}\ncppsort took {:.6f}\nli.sort took {:.6f}'.format(t1 - t0, t2 - t1, t3 - t2))
    return (t3 - t2) / (t2 - t1) - 1


if __name__ == '__main__':
    gain = demo_usage(10, 2 << 20, True)
    print('gain {:.2f}%'.format(gain * 100))
