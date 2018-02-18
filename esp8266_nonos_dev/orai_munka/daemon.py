

import os
import sys
import socket
import thread
import time

def EspSocketInit(port):
    global EspSocket 
    EspSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    EspSocket.bind(('', port))
    #EspSocket.listen(2)

def EspDataSocketInit(port):
    global EspSocketData 
    EspSocketData = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    EspSocketData.bind(('', port))
    EspSocketData.listen(2)

def EspSocketHandler():
    global EspSocketData
    while True:
        conn, data = EspSocketData.accept()
        print "Data received: ", data
        conn.sendto("DAEMON_ACK", ("192.168.1.52", 13000))


def EspSocketHandler():
    global EspSocketData
    while True:
        conn, data = EspSocketData.accept()
        print "Data received: ", data
        conn.sendto("DAEMON_ACK", ("192.168.1.52", 13000))

print "IoT daemon start"
EspCounter = 0
EspSocketInit(13000)
EspDataSocketInit(13001)
EspSocketThr = thread.start_new_thread(EspSocketHandler, ())

try:
    while True:
        EspCounter += 1
        print "Pip", EspCounter
        time.sleep(1)

except KeyboardInterrupt:
    sys.exit()