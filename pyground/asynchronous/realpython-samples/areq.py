#!/usr/bin/env python3
# areq.py

"""
Asynchronously get links embedded in multiple pages' HMTL.
"""

import re
import sys
import asyncio
import logging
from typing import IO
import urllib.error
import urllib.parse
import aiofiles
import aiohttp


logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s %(levelname)-8s %(name)-9s %(message)s', datefmt='%H:%M:%S')
logger = logging.getLogger('areq')
logging.getLogger('chardet.charsetprober').disabled = True

HREF_REGEX = re.compile(r'href="(.*?)"')

# is just a shorthand
Cs = aiohttp.ClientSession


async def fetch_html(url: str, session: Cs, **kwargs) -> str:
    """
        GET request wrapper to fetch page HTML.
        kwargs are passed to `session.request()`
    """

    resp = await session.request(method='GET', url=url, **kwargs)
    resp.raise_for_status()
    logger.info(f'got response [{resp.status}] for URL: {url}')
    html = await resp.text()
    return html


async def parse(url: str, session: Cs, **kwargs) -> set:
    """
        Find HREFs in the HTML of `url`
    """
    found = set()
    try:
        html = await fetch_html(url=url, session=session, **kwargs)
    except (aiohttp.ClientError, aiohttp.http_exceptions.HttpProcessingError) as e:
        ms_s, ms_m = [getattr(e, k, None) for k in ['status', 'message']]
        logger.error(f'aiohttp exception for {url} [{ms_s}]: {ms_m}')
        return found
    except Exception as e:
        ms = getattr(e, '__dict__', {})
        logger.exception(f'non-aiohttp exception occured: {ms}')
        return found
    else:
        for link in HREF_REGEX.findall(html):
            try:
                abslink = urllib.parse.urljoin(url, link)
            except (urllib.error.URLError, ValueError):
                logger.exception(f'error parsing URL {link}')
                pass
            else:
                found.add(abslink)
        logger.info(f'found {len(found)} links for URL {url}')
        return found


async def write_one(file: IO, url: str, **kwargs) -> None:
    """
        Write the found HREFs from `url` to `file`
    """
    res = await parse(url=url, **kwargs)
    if not res:
        return None
    async with aiofiles.open(file, 'a') as f:
        for p in res:
            await f.write(f'{url}\t{p}\n')
        logger.info(f'wrote results for source URL {url}')


async def bulk_crawl_and_write(file: IO, urls: set, **kwargs) -> None:
    """
        Crawl & write concurrently to `file` for multiple `urls`
    """
    async with Cs() as session:
        tasks = []
        for url in urls:
            tasks.append(
                write_one(file=file, url=url, session=session, **kwargs)
            )
        await asyncio.gather(*tasks)


if __name__ == '__main__':
    import pathlib
    import sys

    assert sys.version_info >= (3, 7), 'Script requires Python 3.7+.'
    here = pathlib.Path(__file__).parent

    with open(here.joinpath('urls.txt')) as infile:
        urls = set(map(str.strip, infile))

    outpath = here.joinpath('foundurls.txt')
    with open(outpath, 'w') as outfile:
        outfile.write('source_url\tparsed_url\n')

    asyncio.run(bulk_crawl_and_write(file=outpath, urls=urls))
