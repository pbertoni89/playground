import aiofiles
import asyncio


async def demo():
    async with aiofiles.open('filename', mode='r') as f:
        contents = await f.read()
    print(contents)

loop = asyncio.get_event_loop()
loop.run_until_complete(demo())
