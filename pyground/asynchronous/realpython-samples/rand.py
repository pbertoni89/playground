#!/usr/bin/env python3

"""
This program uses one main coroutine, makerandom(), and runs it concurrently across 3 different inputs.
Most programs will contain small, modular coroutines and one wrapper function that serves to chain each of the smaller coroutines together.

main() is then used to gather tasks (futures) by mapping the central coroutine across some iterable or pool.

In this miniature example, the pool is range(3). In a fuller example presented later, it is a set of URLs that need to be requested, parsed, and processed concurrently, and main() encapsulates that entire routine for each URL.

While “making random integers” (which is CPU-bound more than anything) is maybe not the greatest choice as a candidate for asyncio, it’s the presence of asyncio.sleep() in the example that is designed to mimic an IO-bound process where there is uncertain wait time involved. For example, the asyncio.sleep() call might represent sending and receiving not-so-random integers between two clients in a message application.
"""

import time
import random
import logging
import asyncio


# ANSI colors
c = (
    "\033[0m",   # End of color
    "\033[36m",  # Cyan
    "\033[91m",  # Red
    "\033[35m",  # Magenta
)

logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(name)s %(message)s', datefmt='%H:%M:%S')


async def makerandom(idx: int, threshold: int = 6) -> int:
    lg = logging.getLogger(f'mr_{idx}')
    slp = idx + 1
    lg.info(c[slp] + f'initiated with threshold {threshold}' + c[0])
    i = random.randint(0, 10)
    while i <= threshold:
        lg.info(c[slp] + f'{i} too low; sleep {slp}' + c[0])
        await asyncio.sleep(slp)
        if i <= 2:
            lg.warning(c[slp] + f'{i} very low; sleep again {slp}' + c[0])
            # time.sleep(slp)  # blocca comunque tutto
            await asyncio.sleep(slp)
        i = random.randint(0, 10)
    lg.info(c[slp] + f'---> finished: {i}' + c[0])
    return i


async def main() -> tuple:
    res = await asyncio.gather(*(makerandom(i, 10 - i - 1) for i in range(3)))
    return res


if __name__ == "__main__":
    random.seed(444)
    r1, r2, r3 = asyncio.run(main())
    logging.info(f"r1: {r1}, r2: {r2}, r3: {r3}")
