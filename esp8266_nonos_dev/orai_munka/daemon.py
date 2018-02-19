

import os
import sys
import socket
import thread
import time

class espHeartBeat():
    def __init__(self,port):
        self.port = port
        self.espsocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.espsocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.espsocket.bind(('', self.port))
        self.time_to_die = 0
        self.conn = 0
        self.addr = 0
        self.thread = thread.start_new_thread(self.SocketCbk,())
        print "init socket done"

    def SocketCbk(self):
        while self.time_to_die == 0:
            data = self.espsocket.recv(1024)
            print "HeartBeat, data: %s" %data
            self.espsocket.sendto("DAEMON_OK", ("192.168.1.50",self.port))
    
    def StopSocket(self):
        self.espsocket.close()
        self.time_to_die = 1
        self.thread.join()
class espDataGet():
    def __init__(self,port):
        self.port = port
        self.espsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.espsocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.espsocket.bind(('', self.port))
        self.espsocket.listen(3)
        self.time_to_die = 0
        self.conn = 0
        self.addr = 0
        self.CMD = "DHT_GET"
        self.cmdsts = 0
        self.thread = thread.start_new_thread(self.SocketCbk,())
        print "esp data socket init done"

    def SocketCbk(self):
        while self.time_to_die == 0:
            self.conn, self.addr = self.espsocket.accept()
            data = self.conn.recv(1024)
            print "data received, %s" % data
            if data.find("DAEMON_ASK") > -1 and self.cmdsts == 0:
                print "sending request"
                self.conn.send(self.CMD)
                self.cmdsts == 1
            if data.find("TEMP") > -1 and self.cmdsts == 1:
                print "ACK requested"
                self.conn.send("OK")
                self.cmdsts = 0
    
    def ResetSocket(self):
        self.cmdsts = 0


print "IoT daemon start"
EspCounter = 0
espHeartBeatHndl = espHeartBeat(13000)
espDataHndl = espDataGet(13001)

try:
    while True:
        EspCounter += 1
        print "Pip", EspCounter
        time.sleep(1)

except KeyboardInterrupt:
    sys.exit()