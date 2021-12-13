import re
import numpy
import datetime
import itertools
import subprocess


def get_call(bs: str, count: int) -> list:
    return f'dd if=/dev/zero of=/media/xnext/DATACARM/test3.img bs={bs} count={count} oflag=dsync'.split()


def reg_sample(reg):
    sample = b'100+0 records in\n100+0 records out\n1024000 bytes (1,0 MB, 1000 KiB) copied, 4,33746 s, 236 kB/s\n'
    sample = sample.decode('ASCII')
    sample = sample.split('\n')[-2]
    # print(sample)
    m = reg.match(sample)
    if m:
        rec_time = re.sub(r',', '.', m.group(1))
        rec_rate = int(m.group(2))
        if m.group(3) == 'M':
            rec_rate *= 1024
        print(f'{rec_time},{rec_rate}')


def get_sample(reg: re.Pattern, bs: str, count: int) -> tuple:
    o = subprocess.check_output(get_call(bs, count), stderr=subprocess.STDOUT)
    o = o.decode('ASCII').split('\n')[-2]
    m = reg.match(o)
    if not m:
        raise ValueError(f'missing regex for | {o}')
    rec_time = float(re.sub(r',', '.', m.group(1)))
    rec_rate = float(re.sub(r',', '.', m.group(2)))
    if m.group(3) == 'M':
        rec_rate *= 1024
    return rec_time, int(rec_rate)


def main():
    niter = 10
    lbs = ['1K', '10K', '100K', '1M', '10M']
    lcount = [1, 100]
    reg = re.compile(r'^.*copied, (\d+,\d+) s, (\d+[,\d+]*) (\w)B/s$')
    # reg_sample(reg)
    print('start,end,bs,count,time,rate')

    for bs, count in itertools.product(lbs, lcount):
        date_start = datetime.datetime.now()
        values = [get_sample(reg, bs, count) for _ in range(niter)]
        rec_times = [t for t, _ in values]
        rec_rates = [r for _, r in values]
        rec_time, rec_rate = [numpy.mean(x) for x in [rec_times, rec_rates]]
        date_end = datetime.datetime.now()
        print(f'{date_start},{date_end},{bs},{count},{rec_time: .4f},{rec_rate: .4f}')


if __name__ == '__main__':
    """
        start,end,bs,count,time,rate
        2021-07-28 12:53:32.682347,2021-07-28 12:53:33.212451,1K,1, 0.0499, 21.6000
        2021-07-28 12:53:33.212514,2021-07-28 12:54:22.930250,1K,100, 4.9683, 20.2000
        2021-07-28 12:54:22.930333,2021-07-28 12:54:23.473925,10K,1, 0.0508, 219.4000
        2021-07-28 12:54:23.474011,2021-07-28 12:55:13.964937,10K,100, 5.0449, 203.1000
        2021-07-28 12:55:13.964984,2021-07-28 12:55:14.475234,100K,1, 0.0478, 2323.8000
        2021-07-28 12:55:14.475282,2021-07-28 12:56:06.852625,100K,100, 5.2322, 2017.1000
        2021-07-28 12:56:06.852670,2021-07-28 12:56:07.631015,1M,1, 0.0743, 15154.9000
        2021-07-28 12:56:07.631067,2021-07-28 12:57:12.078613,1M,100, 6.4281, 16701.1000
        2021-07-28 12:57:12.078689,2021-07-28 12:57:13.997603,10M,1, 0.1833, 59657.7000
        2021-07-28 12:57:13.997694,2021-07-28 13:00:07.252718,10M,100, 17.2307, 62299.7000
    """
    main()
