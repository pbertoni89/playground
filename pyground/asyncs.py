#/usr/bin/env python3

import asyncio
import datetime


@asyncio.coroutine
def display_date_1(loop):
    end_time = loop.time() + 5.0
    while True:
        print(datetime.datetime.now())
        if (loop.time() + 1.0) >= end_time:
            break
        yield from asyncio.sleep(1)


async def display_date_2(loop):
    end_time = loop.time() + 5.0
    while True:
        print(datetime.datetime.now())
        if (loop.time() + 1.0) >= end_time:
            break
        await asyncio.sleep(1)


mode = 1
f = display_date_1 if mode == 1 else display_date_2


print('.................0')
loop = asyncio.get_event_loop()
print('.................1')
loop.run_until_complete(f(loop))  # returns when the display_date() coroutine is done
print('.................2')

asyncio.ensure_future(f(loop))

loop.close()
print('.................3')







