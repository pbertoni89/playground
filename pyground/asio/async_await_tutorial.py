#!/usr/bin/env python3

import signal
import sys
import asyncio
import aiohttp
import json
import time
import random
import logging


logging.basicConfig(level=logging.DEBUG, format="%(asctime)s:%(levelname)s:   %(message)s")


loop = asyncio.get_event_loop()
_client = aiohttp.ClientSession(loop=loop)


# - - - DEMO FIBO - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


async def fiboit(n):
    logging.info('fibo {} sleep'.format(n))
    # time.sleep(n)
    a, b = 1, 1
    for i in range(n - 1):
        a, b = b, a + b
    #print('fibo {} -> {}'.format(n, a))
    logging.info('fibo ({}) = {}'.format(n, a))
    return a


async def spawn_fibonacci(n):
    logging.info('fibo {} start'.format(n))
    #async with fiboit(n) as F:
    #    return await F
    return await fiboit(n)


# - - - DEMO REDDIT - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

async def get_json(client, url):
    async with client.get(url) as response:
        assert response.status == 200
        return await response.read()


async def get_reddit_top(subreddit, client):
    data1 = await get_json(client, 'https://www.reddit.com/r/' + subreddit + '/top.json?sort=top&t=day&limit=5')

    j = json.loads(data1.decode('utf-8'))
    for i in j['data']['children']:
        score = i['data']['score']
        title = i['data']['title']
        link = i['data']['url']
        print(str(score) + ': ' + title + ' (' + link + ')')

    logging.info('DONE:', subreddit + '\n')


# - - - DEMO FACTORIAL & GATHER - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# https://docs.python.org/3/library/asyncio-task.html#asyncio-example-gather


async def factorial(name, number):
    f = 1
    for i in range(2, number + 1):
        print(f"Task {name}: Compute factorial({i})...")
        await asyncio.sleep(1)
        f *= i
    print(f"Task {name}: factorial({number}) = {f}")


async def mngr_factorial():
    """
        Schedule three calls *concurrently*
    """
    await asyncio.gather(
        factorial("A", 2),
        factorial("B", 3),
        factorial("C", 4),)


# - - - MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


def signal_handler(signal, frame):
    logging.warning('SIGINT received')
    loop.stop()
    _client.close()
    sys.exit(0)


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    demo_reddit, demo_fibo, demo_fact = False, False, True

    if demo_reddit:
        asyncio.ensure_future(get_reddit_top('python', _client))
        asyncio.ensure_future(get_reddit_top('programming', _client))
        asyncio.ensure_future(get_reddit_top('compsci', _client))
    if demo_fibo:
        asyncio.ensure_future(spawn_fibonacci(7))
        asyncio.ensure_future(spawn_fibonacci(2))
        asyncio.ensure_future(spawn_fibonacci(3))
    if demo_fact:
        asyncio.ensure_future(mngr_factorial())

    logging.info('running forever')
    loop.run_forever()
    logging.info('ran forever, this is the doomsday')
