import thread
import os, sys
from threading import Timer,Thread,Event
import datetime
import time 
import socket
import multiprocessing
##-------------------------------------------------------------------------------
#offline config, global variables


import asyncore
import socket

class threadTimer():

   def __init__(self,t,hFunction):
      self.t=t
      self.hFunction = hFunction
      self.thread = Timer(self.t,self.handle_function)

   def handle_function(self):
      self.hFunction()
      self.thread = Timer(self.t,self.handle_function)
      self.thread.start()

   def start(self):
      self.thread.start()

   def cancel(self):
      self.thread.cancel()
'''
def espHeartBeatSocketInit():
	global esp_s_handler
	esp_s_handler = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	esp_s_handler.bind(('', 13000))
	#esp_s_handler.listen(3)
	print "init done socket"
	
def espHeartBeatSocketCbk():
	global esp_s_handler
	while time_to_die == 0:
		#conn, addr = esp_s_handler.accept()
		data = esp_s_handler.recv(1024)
		print ("some data received", data)
		esp_s_handler.sendto("DAEMON_OK", ("192.168.1.52",13000))
'''

class espHeartBeat():
	def __init__(self, port):
		self.port = port
		self.esp_s_handler = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.esp_s_handler.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.esp_s_handler.bind(('', 13000))
		self.time_to_die = 0
		self.conn = 0
		self.addr = 0
		self.thread = thread.start_new_thread(self.SocketCbk, ())
		#esp_s_handler.listen(3)
		print "init done socket"
	
	def SocketCbk(self):
		while self.time_to_die == 0:
			data = self.esp_s_handler.recv(1024)
			print ("HeartBeat UDP data received", data)
			self.esp_s_handler.sendto("DAEMON_OK", ("192.168.1.50",13000))
	
	def StopSocket(self):
		self.time_to_die = 1
		self.esp_s_handler.close()
		self.thread.join()

class espDataGet():
	def __init__(self, port):
		self.port = port
		self.esp_s_handler = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.esp_s_handler.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.esp_s_handler.bind(('', port))
		self.time_to_die = 0
		self.conn = 0
		self.cmdsts = 0
		self.CMD = "DHT_GET"
		self.thread = thread.start_new_thread(self.ReadSocket, ())
		self.esp_s_handler.listen(3)
		#esp_s_handler.listen(3)
		print "init done socket"
	
	def ReadSocket(self):
		while self.time_to_die == 0:
			self.conn, self.addr = self.esp_s_handler.accept()
			data = self.conn.recv(1024)
			print ("Data TCP data received", data)
			if data.find("DAEMON_ASK") > -1 and self.cmdsts == 0:
				print("Sending request")
				self.conn.send(self.CMD)
				self.cmdsts = 1
			if data.find("TEMP") > -1 and self.cmdsts == 1:
				print("ACK Request")
				self.conn.send("OK")
				self.cmdsts = 0
		
	def StopSocket(self):
		self.time_to_die = 1
		self.esp_s_handler.close()
		self.thread.join()

def measure_job():
	print "asd"

def nullign(handler):
	handler.cmdsts = 0
	print "nullign done"

##-------------------------------------------------------------------------------
#start of the main program
if __name__ == "__main__":
	print ("startup of the Weather Monitor system...initalizing")

	esp_counter = 0

	print ("MainThread start")

	HeartBeat = espHeartBeat(13000)
	Dataget = espDataGet(13001)
	#espHeartBeatSocketInit()
	#espHeartBeatsocket = thread.start_new_thread(espHeartBeatSocketCbk, ())
	#t = threadTimer(10, nullign(Dataget))
	#t.start()
	try:
		while True:
			
			time.sleep(1)
			'''
			esp_counter += 1
			if esp_counter > 10:
				esp_alive = 0
			else:
				esp_alive = 1
			print ("pip")
			#print (esp_counter ,esp_dht_t,esp_dht_h,esp_lm35_t ,esp_alive)
			time.sleep(1)
			print ("papip")
			#measure_job(test)
			'''
	except KeyboardInterrupt:
		print ("daemon stopping")
		Dataget.StopSocket()
		HeartBeat.StopSocket()
		sys.exit()

