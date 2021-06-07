import asyncio

"""
	REQUIRES PYTHON 3.7 or HIGHER
"""

async def hw():
    print('Hello ...')
    await asyncio.sleep(1)
    print('... World!')


if __name__ == '__main__':
	asyncio.run(hw())
