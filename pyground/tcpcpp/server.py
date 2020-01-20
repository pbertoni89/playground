import socket
import time
import struct
import traceback
import numpy
from collections import namedtuple

"""
Sources:
    https://stackoverflow.com/questions/14620632/python-socket-sending-structhaving-c-stuct-as-example
    https://www.binarytides.com/python-socket-server-code-example/
    https://docs.python.org/3/library/struct.html#format-characters
"""


class Req:
    def __init__(self, datab: bytes):
        print('Req from {} bytes {}'.format(len(datab), datab))
        self.__format = '<QiQ'
        self.__struct = struct.unpack(self.__format, datab)  # type: tuple
        self.blob_id, self.px_start, self.payld_sz = [self.__struct[i] for i in range(3)]
        self.payload = numpy.ndarray((1,))
        print('got Req header {}'.format(self))

    def __repr__(self):
        m = 'id {}, px {}, payld_sz {}, payload {}'.format(self.blob_id, self.px_start, self.payld_sz, self.payload)
        return 'Req{' + m + '}'

    def unpack_payload(self, datab: bytes):
        self.payload = numpy.frombuffer(datab, dtype=numpy.uint8)
        # self.payload = numpy.reshape(self.payload, (4, 4))
        print('got Req payload {}'.format(self))


class Res:
    def __init__(self, blob_id, px_start, verdict):
        self.__format = '<Qii'
        self.__ntup = namedtuple('MleRes', 'blob_id px_start verdict')
        self.__struct = self.__ntup(blob_id=blob_id, px_start=px_start, verdict=verdict)
        print('got Res struct {}'.format(self.__struct))

    def pack(self) -> bytes:
        datab = struct.pack(self.__format, *self.__struct._asdict().values())
        print('Res to {} bytes {}'.format(len(datab), datab))
        return datab


def _parse_text(datab: bytes) -> str:
    data = datab.decode()
    if not data:
        raise ValueError('garbage received')
    text = data.strip()
    print('got text: {}'.format(text))
    return text


class FrontendSocket:
    def __init__(self, srv_port):
        """
            :param srv_port:
        """
        hostport = ('', srv_port)  # <blank> host means all available network interfaces)
        # Server Socket: try few times to setup, then surrender
        attempts_left, attempts_sleep = 10, 2
        self.__is_living = True
        self.__res_err = Res(blob_id=0, px_start=0, verdict=0)
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
        run_conn, sz_header = True, 20  # todo change if needed
        while self.__is_living and run_conn:
            print('\nwaiting for {} bytes of header...'.format(sz_header))
            datab = conn.recv(sz_header)
            res = self.__res_err
            try:
                req = Req(datab)
                print('waiting for {} bytes of payload...'.format(req.payld_sz))
                datab = conn.recv(req.payld_sz)
                req.unpack_payload(datab)
                # todo build blob or whatever
                # todo issue predict()
                res = Res(blob_id=req.blob_id, px_start=req.px_start, verdict=1)
            except struct.error as se:
                print('while parsing struct: {}'.format(se))
                text = _parse_text(datab)
                if text == 'close-conn':
                    print('requested: closing connection')
                    run_conn = False
                if text == 'close-server':
                    print('requested: closing server')
                    self.__is_living = False
            finally:
                reply = res.pack()
                conn.sendall(reply)

    def run(self):
        while self.__is_living:
            conn, addr = self.__sock.accept()
            print('connected with {}'.format(':'.join(addr[0:1])))
            try:
                self._conn_life(conn)
            except ValueError as ve:
                print('connection error: {}'.format(ve))
                traceback.print_exc()
            finally:
                print('closing connection {}'.format(':'.join(addr[0:1])))
                conn.close()
        # todo one can issue a shutdown here to simulate UPS shutdown script


if __name__ == '__main__':
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('port', type=int, help='server port')
    args = parser.parse_args()

    fs = FrontendSocket(args.port)
    fs.run()
