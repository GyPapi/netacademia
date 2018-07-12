'''
Server listen folyamatosan, non-blocking üzemmódban (thread)
Csatlakozás fogadása, majd adattartalom validálás után
Küldjön vissza egy JSON-t, amiben legyen benne a dátum, a küldött adat, a válasz, és némi random szám.

'''

import socket
import json
import time
import random
import threading

def getToken():
    output = []
    for x in range(0, 9):
        output.append(random.randint(1,200))
    return output

class Server(object):
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))
        print("server init done")

    def listen(self):
        self.sock.listen(5)
        while True:
            client, address = self.sock.accept()
            print("accepting connection from ", address, " client ", client)
            client.settimeout(60)
            threading.Thread(target=self.listenToClient, args=(client,address)).start()

    def listenToClient(self, client, address):
        size = 1024
        while True:
            try:
                data = client.recv(size)
                if data:
                    try:
                        answer = {"asd": 3}
                        answer['echo_payload'] = data.decode('ascii')
                        answer['secrets'] = []
                        answer['secrets'].append(getToken())
                        answer['datetime'] = time.strftime("%Y-%M-%d %H:%M:%s")
                        print(answer)
                        serialized = json.dumps(answer)
                        print(serialized)
                    except(TypeError, ValueError):
                        print("error in JSON parsing")
                    client.send(b"%d\n" % len(serialized))
                    client.sendall(serialized.encode('ascii'))
                    '''
                    print("data received," , data)
                    client.send(b"POP")
                    '''
            except:
                client.close()
                print("closing....")
                return False

if __name__ == "__main__":
    random.seed(23525)
    print("Starting server")
    while True:
        Server('', 11500).listen()
        print("loop!!")
        time.sleep(2)


