#!/usr/bin/env python2
# -*- coding: utf-8 -*

# Copyright (c) Twisted Matrix Laboratories.
# See LICENSE for details.


from asio.internet import reactor, protocol


class Echo(protocol.Protocol):
    """
        This is just about the simplest possible protocol
    """

    def dataReceived(self, data):
        """
            As soon as any data is received, write it back
            :param data:
            :return:
        """
        self.transport.write(data)


# This runs the protocol on port 8000
def main():
    factory = protocol.ServerFactory()
    factory.protocol = Echo
    reactor.listenTCP(8000, factory)
    reactor.run()


# this only runs if the module was *not* imported
if __name__ == '__main__':
    main()
