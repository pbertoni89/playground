import socket
import time
import struct
from collections import namedtuple

"""
Sources:
    https://www.binarytides.com/python-socket-server-code-example/
    https://docs.python.org/2/library/struct.html#format-characters
"""


class Req:
    def __init__(self, datab: bytes):
        self.__format = '<Qi'
        self.__struct = struct.unpack(self.__format, datab)  # type: tuple
        self.tc_start, self.px_start = self.__struct[0], self.__struct[1]
        # nt_gen = namedtuple('MleReq', 'tcStart pxStart')
        # self.__nt = nt_gen(tcStart=10, pxStart=12)
        # print('got Req struct {}'.format(self.__nt))
        # print('got Req field {}'.format(self.__nt.tcStart))


class Res:
    def __init__(self, timecode_start, pixel_start, verdict):
        self.__format = '<Qii'
        self.__ntup = namedtuple('MleRes', 'tcStart pxStart verdict')
        self.__struct = self.__ntup(tcStart=timecode_start, pxStart=pixel_start, verdict=verdict)
        print('got Res struct {}'.format(self.__struct))

    def pack(self) -> bytes:
        reply = struct.pack(self.__format, *self.__struct._asdict().values())
        print('replying bytes {}'.format(reply))
        # print('i.e. ts {}, ps {}, v {}'.format(self.__ntup.tcStart, self.__ntup.pxStart, self.__ntup.verdict))
        # print('i.e. {}'.format(self.__ntup))
        return reply


class FrontendSocket:
    def __init__(self, srv_port):
        """
            :param srv_port:
        """
        hostport = ('', srv_port)  # <blank> host means all available network interfaces)
        # Server Socket: try few times to setup, then surrender
        attempts_left, attempts_sleep = 10, 2
        self.__sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        while attempts_left > 0:
            try:
                self.__sock.bind(hostport)
                self.__sock.listen(5)  # accept up to N connections
                print('listening on {}:{}'.format(hostport[0], hostport[1]))
                attempts_left = -1
            except OSError as ose:
                if attempts_left == 0:
                    raise
                print('{}. Attempts left: {}'.format(ose, attempts_left))
                attempts_left -= 1
                time.sleep(attempts_sleep)

    def __del__(self):
        print('closing socket')
        try:
            self.__sock.shutdown(socket.SHUT_RDWR)
            self.__sock.close()
        except OSError as ose:
            print(ose)

    def _conn_life(self, conn: socket.socket):
        run_conn = True
        while run_conn:
            print('')
            datab = conn.recv(1024)  # todo change with actual spectra size
            try:
                req = Req(datab)
                stru = struct.unpack('<Qi', datab)
                print('got struct: {}'.format(stru))
            except struct.error:
                data = datab.decode()
                if not data:
                    raise ValueError('garbage received')
                text = data.strip()
                if text == 'exit':
                    print('correct exit procedure')
                    run_conn = False
                print('got text: {}'.format(text))
            finally:
                reply = self._reply()
                conn.sendall(reply)

    def run(self):
        run_mle = True
        while run_mle:
            conn, addr = self.__sock.accept()
            print('connected with {}'.format(':'.join(addr[0:1])))
            try:
                self._conn_life(conn)
            except ValueError as ve:
                print(ve)
            finally:
                print('closing connection {}'.format(':'.join(addr[0:1])))
                conn.close()
        # todo one can issue a shutdown here to simulate UPS shutdown script

    def _reply(self) -> bytes:
        """
            https://stackoverflow.com/questions/14620632/python-socket-sending-structhaving-c-stuct-as-example
            :return bytes to send
        """
        resp = Res(timecode_start=56, pixel_start=78, verdict=100)
        return resp.pack()


if __name__ == '__main__':
    fs = FrontendSocket(8889)
    fs.run()
