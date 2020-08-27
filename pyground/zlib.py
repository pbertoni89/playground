#!/usr/bin/python3-xnext
# -*- coding: utf-8 -*-
#

import zlib
import binascii


def demo():
    data = 'Hello world'
    compressed_data = zlib.compress(data, 2)
    print('Original data: ' +  data)
    print('Compressed data: ' + binascii.hexlify(compressed_data))


def demo_compress():
    s = '...'
    z = zlib.compress(bytes(s), 1)
    with open('/tmp/savepy.z', 'w') as f:
        f.write(z)


def demo_decompress():
    fnsrc = '/tmp/savecpp.z'
    with open(fnsrc, 'rb') as f:
        z = f.read()
    s = zlib.decompress(z)
    print(s)


if __name__ == '__main__':
    demo_compress()
    # demo_decompress()
    # demo()
