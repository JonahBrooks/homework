## report.py
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
import sys


# Globals
ID_REPORT = 1l
SNT_KILL = 1l
SNT_REPORT = 2l

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

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


def signal_handler(sig_num, frame):
	print "DIE DIE DIE!" 
	try:	
		sock.shutdown(0)
		sock.close()
	except socket.error:
		pass
	signal.signal(sig_num, signal.SIG_DFL)
	os.kill(os.getpid(), sig_num)

signal.signal(1, signal_handler)
signal.signal(2, signal_handler)
signal.signal(3, signal_handler)



if len(sys.argv) == 2:
	host = sys.argv[1]
	port = 15048
	sock.connect((host, port))
	sock.send(encode(ID_REPORT))
	sock.send(encode(SNT_REPORT))

	report_struct = struct.Struct('!I16p')
	
	msg = sock.recv(20)
	while msg:
		num, ip = report_struct.unpack(msg)
		if "checked" in ip:
			print "\t\t",num, ip
		else:
			print "A user at IP address:", ip, " found", num, "to be a perfect number"
		msg = sock.recv(20)
	
	sock.shutdown(0)
	sock.close()
elif (len(sys.argv) == 3) and ("-k" in sys.argv): 
	host = sys.argv[1]
	port = 15048
	sock.connect((host, port))
	sock.send(encode(ID_REPORT))
	sock.send(encode(SNT_KILL))
	print "Kill request confirmed. Manage has been informed of its impending doom."
else:
	print "Please enter the hostname as a command line argument"





