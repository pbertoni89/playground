#!/usr/bin/env python3
# chained.py


import asyncio
import random
import logging
import time


logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(name)s %(message)s', datefmt='%H:%M:%S')


async def part1(n: int) -> str:
    i = random.randint(0, 10)
    logging.info(f"part1({n}) sleeping for rand {i} s")
    await asyncio.sleep(i)
    result = f"result{n}-1"
    logging.info(f"returning part1({n}) == {result}.")
    return result


async def part2(n: int, arg: str) -> str:
    i = random.randint(0, 10)
    logging.info(f"part2{n, arg} sleeping for rand {i} s")
    await asyncio.sleep(i)
    result = f"result{n}-2 derived from {arg}"
    logging.info(f"returning part2{n, arg} == {result}.")
    return result


async def chain(n: int) -> None:
    start = time.perf_counter()
    p1 = await part1(n)
    p2 = await part2(n, p1)
    end = time.perf_counter() - start
    logging.info(f"-->chained result{n} => {p2} (took {end:0.2f} s)")


async def main(*args):
    await asyncio.gather(*(chain(n) for n in args))


if __name__ == "__main__":
    import sys
    random.seed(444)
    args = [1, 2, 3] if len(sys.argv) == 1 else map(int, sys.argv[1:])
    start = time.perf_counter()
    asyncio.run(main(*args))
    end = time.perf_counter() - start
    logging.info(f"program finished in {end:0.2f} s")
