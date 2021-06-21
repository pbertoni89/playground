#!/usr/bin/env python3

import time
import os
import random
import asyncio
import logging
import itertools as it

"""
The synchronous version of this program would look pretty dismal: a group of blocking producers serially add items to the queue, one producer at a time. Only after all producers are done can the queue be processed, by one consumer at a time processing item-by-item. There is a ton of latency in this design. Items may sit idly in the queue rather than be picked up and processed immediately.

An asynchronous version, asyncq.py, is below. The challenging part of this workflow is that there needs to be a signal to the consumers that production is done. Otherwise, await q.get() will hang indefinitely, because the queue will have been fully processed, but consumers won’t have any idea that production is complete.
"""

logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(name)-10s %(message)s', datefmt='%H:%M:%S')


async def makeitema(size: int = 5) -> str:
    """
        There's no strict need for it, actually. But it shows how endless the async/await chain can be
        You could also write makeitems
    """
    return os.urandom(size).hex()


async def randsleep(caller=None, low=0, high=4, mult=1.) -> None:
    """
        Keep in mind that asyncio.sleep() is used to mimic some other, more complex coroutine that would eat up time and block all other execution if it were a regular blocking function.
    """
    i = random.randint(low, high) / mult
    if caller:
        logging.info(f'{caller} sleeping for {i} s')
    await asyncio.sleep(i)


async def produce(name: int, q: asyncio.Queue) -> None:
    nm = f'prod_{name}'
    lg = logging.getLogger(nm)
    n = random.randint(1, 5)
    lg.info(f'starting... produce {n} items')
    for _ in it.repeat(None, n):  # synchronous loop for each single producer
        await randsleep(caller=nm)  #, low=3, high=6)
        i = await makeitema()
        t = time.perf_counter()
        await q.put((i, t))
        lg.info(f'push <{i}> -> size {q.qsize()}')
    lg.info(f'stopping')


async def consume(name: int, q: asyncio.Queue) -> None:
    nm = f'cons_{name}'
    lg = logging.getLogger(nm)
    while True:
        await randsleep(caller=nm)  #, low=5, high=8)
        i, t = await q.get()
        now = time.perf_counter()
        lg.info(f'pop <{i}> in {now - t: 0.5f} s -> size {q.qsize()}')
        q.task_done()


async def main(nprod: int, ncon: int):
    """
        This Queue is a container for tuples (int, float) where
          * int is my data value
          * float is the timestamp of its insertion

        The key is to await q.join(), which blocks until all items in the queue have been received and processed, and then to cancel the consumer tasks, which would otherwise hang up and wait endlessly for additional queue items to appear
    """
    q = asyncio.Queue()
    producers = [asyncio.create_task(produce(n, q)) for n in range(nprod)]
    consumers = [asyncio.create_task(consume(n, q)) for n in range(ncon)]
    await asyncio.gather(*producers)  # let producers end production
    await q.join()                    # implicitly awaits consumers, too
    for c in consumers:
        c.cancel()                    # gently detaches from consumers


if __name__ == '__main__':
    import argparse
    random.seed(444)
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--nprod', type=int, default=5)
    parser.add_argument('-c', '--ncon', type=int, default=10)
    ns = parser.parse_args()
    start = time.perf_counter()
    asyncio.run(main(**ns.__dict__))
    elapsed = time.perf_counter() - start
    logging.info(f'program completed in {elapsed:0.5f} s')
