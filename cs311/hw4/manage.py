## manage.py
## Jonah Brooks
## Assignment 4

## Design! 
## Sentinels:
##	New Clients:
##		0: compute client
##		1: report client
##	Compute Clients:
##		0: Finished succesfully
##		1: Request number range
##	Report Client:
##		0: DDDIIIEEEE
##		1: Request report

## TODO:
##	Implement some way to notify all computes that they should quit
##	Possibly implement the verification system to prevent computes
##		from starting a section then quiting out

import socket
import signal
import os
import select
import struct

#globals
ID_REPORT = 1l
SNT_KILL = 1l
SNT_REPORT = 2l

ID_COMPUTE = 2l
SNT_DONE = 1l
SNT_REQUEST = 2l

ID_WATCHER = 3l

PORT = 15048
HOST = ''
BUFFER_SIZE = 4
server = socket.socket()
next_to_check = 4
max_to_check = 2**32 - 2 
socket_list = []
report_list = []
compute_list = []
watcher_list = []
perfect_numbers = []

def signal_handler(sig_num, frame):
	print "DIE DIE DIE!" # TODO: Remove this.
	for ready_socket in socket_list:
		try:	
			ready_socket.shutdown(0)
			ready_socket.close()
		except socket.error:
			pass
	signal.signal(sig_num, signal.SIG_DFL)
	os.kill(os.getpid(), sig_num)

signal.signal(1, signal_handler)
signal.signal(2, signal_handler)
signal.signal(3, signal_handler)


def report_handler(client):
	global next_to_check
	
	report_struct = struct.Struct('!I16p')

	msg = decode(client.recv(BUFFER_SIZE, socket.MSG_DONTWAIT)) # Remove the noblock
	
	if not msg:
		pass 
	elif msg == SNT_KILL:
		print "No Mr. Manage; I expect you to die."
		os.kill(os.getpid(), 2) # DIIIEEEE
	elif msg == SNT_REPORT:
		for data in perfect_numbers:
			num, ip = data
			report = report_struct.pack(num, ip)
			sent = 0
			while sent < len(report):
				sent = sent + client.send(report[sent:])
		report = report_struct.pack(next_to_check-1, "Numbers checked")
		sent = 0
		while sent < len(report):
			sent = sent + client.send(report)

	report_list.remove(client)
	socket_list.remove(client)
	client.shutdown(0)
	client.close()



def compute_handler(client):
	global next_to_check, max_to_check
	
	is_closable = False

	# TODO: Either store this clients last start and end somewhere here, or query them from the client	
	#       if I want to verify that the section was completed
	msg = decode(client.recv(BUFFER_SIZE, socket.MSG_DONTWAIT)) # Remove the noblock
	
	if not msg:
		is_closable = True
	elif msg == SNT_DONE:
		# TODO: Flag the previous set as verified
		is_closable = True
	elif msg == SNT_REQUEST:
		# TODO: Flag the previous set as verified
		#print "\t\tIt's requesting a new number"
		start = next_to_check
		end = start
		total = start
		client_IOPS = decode(client.recv(BUFFER_SIZE))
		if not client_IOPS:
			is_closable = True
		else:
			while total < client_IOPS*15:
				end = end + 1
				total = total + end
			if end >= max_to_check:
				end = max_to_check
			next_to_check = end
			#print "\t\tSending it up to", end
			client.send(encode(start))
			client.send(encode(end))
	else:
		# Store the perfect number (with detailed information) in perfect_numbers
		ip, other_thing = client.getpeername()	
		perfect_numbers.append((msg,ip))
		#print "\t\tIt found that", perfect_numbers[len(perfect_numbers)-1], "is perfect."
	
	if is_closable == True:
		compute_list.remove(client)
		socket_list.remove(client)
		client.shutdown(0)
		client.close()
		

def get_external_ip():
	tmp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	tmp.connect(("google.com",80))
	extern_ip, extern_port = tmp.getsockname()
	tmp.shutdown(0)
	tmp.close()
	return extern_ip


def encode(number):
	tmp = socket.htonl(number) 
	chr_num = range(4);
	chr_num[0] = tmp & 255
	chr_num[1] = (tmp >> 8) & 255
	chr_num[2] = (tmp >> 16) & 255
	chr_num[3] = (tmp >> 24) & 255
	str_num = ''
	for num in chr_num:
		str_num += chr(num)
		
	return str_num


def decode(chr_num): 
	tmp = 0L

	if chr_num:
		tmp |= ord(chr_num[0])
		tmp |= ord(chr_num[1]) << 8
		tmp |= ord(chr_num[2]) << 16
		tmp |= ord(chr_num[3]) << 24

	return socket.ntohl(tmp)


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

HOST = get_external_ip()
server.bind((HOST, PORT)) 
print "New server socket at address", HOST,	"on port", PORT
server.listen(5)
socket_list.append(server)

while True:  
	read_ready, write_ready, i_am_error = select.select(socket_list, [], [])
	for ready_socket in read_ready:
		if ready_socket is server:
			(client, addr) = server.accept()
			client_id = decode(client.recv(BUFFER_SIZE))
			if client_id == ID_REPORT:
				print "New report client accepted"
				socket_list.append(client)
				report_list.append(client)
			elif client_id == ID_COMPUTE:
				print "New compute client accepted"
				socket_list.append(client)
				compute_list.append(client)
			elif client_id == ID_WATCHER:
				print "New watcher client accepted"
				socket_list.append(client)
				watcher_list.append(client)
			else:
				print "ERROR: Unidentified client attempting connection."
				client.shutdown(0)
				client.close()
		elif ready_socket in compute_list and next_to_check != max_to_check:
			#print "\tIt's a compute client:"
			compute_handler(ready_socket)
		elif ready_socket in report_list:
			#print "\tIt's a report client"
			report_handler(ready_socket)
		

