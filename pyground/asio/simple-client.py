#!/usr/bin/env python2
# -*- coding: utf-8 -*
#
# Copyright: Xnext s.r.l.
#
# Author: Patrizio Bertoni pbertoni@x-next.com
#
# I'm sick of Bash scripting. This will be the daemon handler
#

# Copyright (c) Twisted Matrix Laboratories.
# See LICENSE for details.
#
# An example client. Run simpleserv.py first before running this.
#

from twisted.internet import reactor, protocol


class EchoClient(protocol.Protocol):
    """
        A client echo protocol.
        Once connected, send a message, then print the result.
    """

    def connectionMade(self):
        self.transport.write(b"hello, world!")

    def dataReceived(self, data):
        """
            As soon as any data is received, write it back
        """
        print("Server said:", data)
        self.transport.loseConnection()

    def connectionLost(self, reason):
        print("connection lost")


class EchoFactory(protocol.ClientFactory):
    protocol = EchoClient

    def clientConnectionFailed(self, connector, reason):
        print("Connection failed - goodbye!")
        reactor.stop()

    def clientConnectionLost(self, connector, reason):
        print("Connection lost - goodbye!")
        reactor.stop()


def main():
    """
        this connects the protocol to a server running on port 8000
    """
    print("~  ~  ~  ~  ~  ~  ~  aborting xspyctrad - if running  ~  ~  ~  ~  ~  ~  ~")
    f = EchoFactory()
    reactor.connectTCP("localhost", 8000, f)
    reactor.run()


# this only runs if the module was *not* imported
if __name__ == '__main__':
    main()
