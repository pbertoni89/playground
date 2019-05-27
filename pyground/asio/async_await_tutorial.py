#!/usr/bin/env python3
import signal
import sys
import asyncio
import aiohttp
import json
import time
import random

loop = asyncio.get_event_loop()
_client = aiohttp.ClientSession(loop=loop)


async def fiboit(n):
    time.sleep(n)
    a, b = 1, 1
    #for i in range(n - 1):
    #    a, b = b, a + b
    #print('fibo {} -> {}'.format(n, a))
    print('fibo {} ok'.format(n))
    return a


async def spawn_fibonacci(n):
    print('fibo {} start'.format(n))
    #async with fiboit(n) as F:
    #    return await F
    return await fiboit(n)


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

    print('DONE:', subreddit + '\n')


def signal_handler(signal, frame):
    print('SIGINT received')
    loop.stop()
    _client.close()
    sys.exit(0)


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    demo_reddit, demo_fibo = False, True

    async def _helloworld():
        print('Hello ...')
        await asyncio.sleep(1)
        print('... World!')
    asyncio.run(_helloworld())

    if demo_reddit:
        asyncio.ensure_future(get_reddit_top('python', _client))
        asyncio.ensure_future(get_reddit_top('programming', _client))
        asyncio.ensure_future(get_reddit_top('compsci', _client))
    if demo_fibo:
        asyncio.ensure_future(spawn_fibonacci(7))
        asyncio.ensure_future(spawn_fibonacci(2))
        asyncio.ensure_future(spawn_fibonacci(3))
    loop.run_forever()
