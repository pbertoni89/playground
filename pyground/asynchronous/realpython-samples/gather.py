#!/usr/bin/env python3

import time
import asyncio
import logging

#
# Showing that synchronous sleeps, which mimick CPU-bound work or poorly written IO-bound work,
# Are opaque to asyncio and can't be boosted.
# Once you go for asyncio, every critical region MUST be async. In facts, it is its rationale
#

logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(name)s %(message)s', datefmt='%H:%M:%S')


async def count_async():
    """
        Talking to each of the calls to count_async() is a single event loop, or coordinator
        ask the event loop to run something else while your await statement finishes its execution
    """
    logging.info('atic ...')
    await asyncio.sleep(1)
    logging.info('... atoc')


async def count_sync():
    """
        block the entire execution of the script
    """
    logging.info('stic ...')
    time.sleep(1)
    logging.info('... stoc')


async def count_async_main(f: callable):
    await asyncio.gather(f(), f(), f())


def count_async_wrap(is_really_async: bool) -> None:
    f = count_async if is_really_async else count_sync
    s = time.perf_counter()
    asyncio.run(count_async_main(f))
    elapsed = time.perf_counter() - s
    logging.info(f'{__file__} executed in {elapsed: 0.2f} seconds with {f.__name__}\n')


if __name__ == '__main__':
    count_async_wrap(True)
    print('')
    count_async_wrap(False)
