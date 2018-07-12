'''
Csatlakozik szerverhez, adatot küld, majd a választ értelmezi, és felhasználja
'''


import socket
import json
import time
import sys

class socketClient(object):
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.csocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.csocket.connect((self.ip, self.port))

    def send(self):
        print("connecting to %s, port %d" % (str(self.ip), self.port))
        self.csocket.send(b"ping")
        len_str = ''
        x = self.csocket.recv(1)
        char = x.decode('ascii')
        while char != '\n':
            len_str += char
            x = self.csocket.recv(1)
            char = x.decode('ascii')
        total = int(len_str)
        print("total bytes: ", total)
        view = memoryview(bytearray(total))
        next_offset = 0
        while total - next_offset > 0:
            recv_size = self.csocket.recv_into(view[next_offset:], total - next_offset)
            next_offset += recv_size
            print("next_offset,", next_offset)
        try:
            deserialized = json.loads(view.tobytes())
        except(TypeError, ValueError):
            print("error in parsing")
        print(deserialized)
        print(deserialized['secrets'])
        return deserialized['datetime']

        '''
        data = self.csocket.recv(1024)
        print("received ", data)
        return data
        '''

    def close(self):
        self.csocket.close()

if __name__ == "__main__":
    try:
        print("client start...")
        sClient = socketClient('localhost', 11500)
        while True:
            answer = sClient.send()
            print(answer)
            time.sleep(1)
    except KeyboardInterrupt:
        print("halting...")
        sClient.close()
        sys.exit(1)