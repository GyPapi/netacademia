import thread
import os, sys
from threading import Timer,Thread,Event
import datetime
import time 
import socket
import multiprocessing
##-------------------------------------------------------------------------------
#offline config, global variables




def PhpEchoHandler():
	global php_echo_h
	global esp_alive 
	global esp_dht_h 
	global esp_dht_t 
	global esp_lm35_t
	while True:
		print (esp_alive,esp_dht_h,esp_dht_t,esp_lm35_t)
		conn,addr = php_echo_h.accept()
		print ("Incoming connection from %s" % repr(addr))
		data = conn.recv(8192)
		if data:
			print ("asked from the socket:",data)
			if data.find("esp_ALIVE") > -1:
				print ("PHP asking for esp, alive?", esp_alive)
				if(esp_alive > 0):
					conn.send("OK:" + str(esp_dht_t)+ ":"+str(esp_dht_h)+ ":"+str(esp_lm35_t))
					print ("esp alive sent")			
			else:
				print ("not handled in the answers")





def espSocketInit():
	global esp_s_handler
	esp_s_handler = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	esp_s_handler.bind(('', 13000))
	#esp_s_handler.listen(3)
	print "init done socket"

def espSocketCbk():
	global esp_s_handler
	while 1:
		#conn, addr = esp_s_handler.accept()
		data = esp_s_handler.recv(1024)
		print ("some data received, not good", data)
		esp_s_handler.sendto("DAEMON_OK", ("192.168.1.52",13000))


##-------------------------------------------------------------------------------
#start of the main program
print ("startup of the Weather Monitor system...initalizing")

esp_counter = 0


print ("MainThread start")

#PhpEchoServer('',12300)
#php_socket = thread.start_new_thread(PhpEchoHandler, ())
#php_socket.start()

espSocketInit()

esp_socket = thread.start_new_thread(espSocketCbk, ())
#esp_socket.start()


#t = perpetualTimer(THREAD_DELAY,measure_job)
#t.start()


try:
	while True:
		espSocketCbk()
		'''
		time.sleep(1)

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
	esp_socket.exit()
	sys.exit()

