import time


def mistery(n):
    span = range(2, n)
    x = [i for i in span]
    for it in span:
        len_bef = len(x)
        x = [i for i in x if i == it or i % it != 0]
        if len(x) == len_bef:
            break
    return x


def geeks(n):
    prime = [True for _ in range(n + 1)]
    p = 2
    while p * p <= n:
        if prime[p]:
            for i in range(p * 2, n + 1, p):
                prime[i] = False
        p += 1
    prime[0] = False
    prime[1] = False


if __name__ == '__main__':
    print(mistery(2 << 6))
    print(mistery(0))
    print(mistery(1))
    print(mistery(2))
    kpi = 1 << 16
    t0 = time.time()
    mistery(kpi)
    t1 = time.time()
    print('mistery run in {}'.format(t1 - t0))
    geeks(kpi)
    t2 = time.time()
    print('geeks run in {}'.format(t2-t1))
