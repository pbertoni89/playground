import asyncio
import datetime

"""
Running BOTH would end in
  File "so.py", line 31, in demo_b
    loop.run_until_complete(display_date_b(loop))
  File "/media/xnext/DATAEXT/opt/miniconda/envs/xs_stable/lib/python3.6/asyncio/base_events.py", line 464, in run_until_complete
    self._check_closed()
  File "/media/xnext/DATAEXT/opt/miniconda/envs/xs_stable/lib/python3.6/asyncio/base_events.py", line 381, in _check_closed
    raise RuntimeError('Event loop is closed')
RuntimeError: Event loop is closed
sys:1: RuntimeWarning: coroutine 'display_date_b' was never awaited

"""

@asyncio.coroutine
def display_date_a(loop):
    end_time = loop.time() + 5.0
    while True:
        print(datetime.datetime.now())
        if (loop.time() + 1.0) >= end_time:
            break
        yield from asyncio.sleep(1)


async def display_date_b(loop):
    end_time = loop.time() + 5.0
    while True:
        print(datetime.datetime.now())
        if (loop.time() + 1.0) >= end_time:
            break
        await asyncio.sleep(1)


def demo_a():
    loop = asyncio.get_event_loop()
    # Blocking call which returns when the display_date() coroutine is done
    loop.run_until_complete(display_date_a(loop))
    loop.close()

def demo_b():
    loop = asyncio.get_event_loop()
    loop.run_until_complete(display_date_b(loop))
    loop.close()

def demo_c():
    loop = asyncio.get_event_loop()
    print('firing a')
    loop.run_until_complete(display_date_a(loop))
    print('firing b')
    loop.run_until_complete(display_date_b(loop))
    print('closing')
    loop.close()


if __name__ == '__main__':
    # demo_a()
    # demo_b()
    demo_c()

