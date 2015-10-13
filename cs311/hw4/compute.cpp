
/*
 * Original Author: Jonah Brooks (brookjon)
 * File: compute.cpp
 * Created: 2012 March 11
 * Last Modified: 2012 March 16 20:04
 *
 * Connects to a server, requests a range
 * of numbers, finds any perfect numbers
 * in that range, then reports any numbers
 * found to the server before requesting
 * another range of numbers
 *
 * Worked with Savannah Van Beek:
 *   Mostly a bit of general discussion
 *   of strategies and coding style.
 *   Occasional exchange of useful links
 *   and resources.
 *
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#include "moo_clock.h"

#define BUFFER_SIZE 4
#define ID_COMPUTE 2l
#define SNT_REQUEST 2l
#define SNT_DONE 1l
#define ID_WATCHER 3l

// TODO:  // Note, I could not get this working the way I intended
//	Create a thread that makes a new socket connecting to the server
//	perhaps identified by the number 2 (increasing the check min to 3)
//	This thread would then, after notifying the server that it's a watcher,
//	exectute a blocking recv. If it hears anything at all, it will hit
//	the kill switch on the program by calling kill.

// watcher thread handler
//void *watcher_handler(void *args);

// signal handlers
void handler(int arg);

// establish connection function ?
void init_connection(int *socket_fd, char* host, char* port);

// convert an unsigned int into a char array of size 5
void encode(unsigned int number, char *chr_num);

// decode the char array
unsigned int decode(char *chr_num);

// time check function
unsigned int find_IOPS();

// query the server for a range of numbers
void fetch_numbers(int socket_fd, unsigned int *start_num, unsigned int *end_num);

// calculate function
bool perfection_checker(unsigned int number);


// globally define sockets, or have handler functions
int sock_fd;
//int watcher_socket_fd;
//pthread_t watcher_thread_id;
char *host;
char port[6] = "15048";


int main(int argc, char *argv[]) {

	signal(1, handler); // HUP
	signal(2, handler); // INT
	signal(3, handler); // QUIT ... technically this should core dump as well..

	//char *host;
	//char *port;

	if (argc == 2) {
		host = argv[1];
	} else {
		std::cout << "Error: Must supply a host name as command line arg" << std::endl;
		return(EXIT_FAILURE);
	}

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	init_connection(&sock_fd, host, port);


	unsigned int start;
	unsigned int end;
	char perfect[BUFFER_SIZE];
	while (end <= UINT_MAX - 2) {
		fetch_numbers(sock_fd, &start, &end);
		moo_clock::tic();
		std::cout << "Checking numbers between " << start << " and " << end << std::endl;
		for (unsigned int i = start; i < end; ++i) {
			if (perfection_checker(i)) {
				encode(i, perfect);
				send(sock_fd, perfect, BUFFER_SIZE, 0);
			} 
		}
		std::cout << "Completed range in " <<  moo_clock::toc() << " seconds." << std::endl;
	}
	encode(SNT_DONE, perfect);
	send(sock_fd, perfect, BUFFER_SIZE, 0);
	std::cout << "All done." << std::endl;
	shutdown(sock_fd, SHUT_RDWR);
	close(sock_fd);
	return 0;
}


void init_connection(int *socket_fd, char* host, char* port) {

	bool connected = false;
	struct addrinfo *addrs;
	struct addrinfo hints;
	char id[BUFFER_SIZE];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;    

	getaddrinfo(host, port, &hints, &addrs);

	for (struct addrinfo *ad = addrs; ad != NULL && connected == false; ad = ad->ai_next) {
		std::cout << "Attempting connection..." << std::endl;
		if (connect(sock_fd, ad->ai_addr, ad->ai_addrlen) == 0) {
			// init compute
			encode(ID_COMPUTE, id);
			send(sock_fd, id, BUFFER_SIZE, 0);
			connected = true;
			//pthread_create(&watcher_thread_id, NULL, watcher_handler, NULL);
		} 
	}
	
	if (connected == true) {
		std::cout << "Connection established." << std::endl;
	} else {
		std::cout << "Connection failed, terminating program." << std::endl;
	}
	
	freeaddrinfo(addrs);
}


// Converts the number into an char array in htonl format
// returns this array by storing it in chr_num
void encode(unsigned int number, char *chr_num) {
	unsigned int htonl_number = htonl(number);
	unsigned int tmp = htonl_number;
	chr_num[0] = tmp & 255; // first char is lowest order
	chr_num[1] = (tmp >> 8) & 255;
	chr_num[2] = (tmp >> 16) & 255;
	chr_num[3] = (tmp >> 24) & 255;

}


// Converts the an char array in htonl format
// returns an unsigned int in ntohl format
unsigned int decode(char *chr_num) {
	unsigned int tmp = 0;

	tmp |= chr_num[0];
	tmp |= chr_num[1] << 8;
	tmp |= chr_num[2] << 16;
	tmp |= chr_num[3] << 24;

	return ntohl(tmp);
}


// Sends the server at socket_fd this programs IOPS number,
// then queries the server a range of numbers, which is to be stored in
// start_num and end_num.
void fetch_numbers(int socket_fd, unsigned int *start_num, unsigned int *end_num) {
	char to_send[BUFFER_SIZE];
	char to_recv[BUFFER_SIZE];
	encode(SNT_REQUEST, to_send); // sentinel signifying a new range request is coming
	send(socket_fd, to_send, BUFFER_SIZE, 0);

	encode(find_IOPS(), to_send);
	send(socket_fd, to_send, BUFFER_SIZE, 0);

	recv(socket_fd, to_recv, BUFFER_SIZE, 0);
	*start_num = decode(to_recv);

	recv(socket_fd, to_recv, BUFFER_SIZE, 0);
	*end_num = decode(to_recv);

}


// Checks number for perfectitude (or a word that exists) 
bool perfection_checker(unsigned int number) {
	unsigned int sum = 1;

	for (unsigned int i = 2; i <= number / 2; ++i) {
		if ((number % i) == 0) {
			sum += i;
		}
	}

	return (number == sum);
}


// Finds the number that can be calculated in one second
unsigned int find_IOPS() {
	unsigned int number = 1000000;
	unsigned int sum = 1;
	unsigned int IO = 0;

	moo_clock::tic();
	while (moo_clock::toc() < 1) {
		for (unsigned int i = 2; i <= number / 2; ++i) {
			if ((number % i) == 0) {
				sum += i;
			}
		}
		IO += number;
	}

	return IO; // Technically IOPS*2, but this makes more sense
}


// Clean up and DIE!
void handler(int arg) {
	shutdown(sock_fd, SHUT_RDWR);
	close(sock_fd);  
	signal(arg, SIG_DFL);
	raise(arg);
}


// THE FOLLOWING CODE, AND ALL COMMENTED CODE ABOVE, DOES NOT FUNCTION CORRECTLY
// They were attempts to create a thread that listens for a kill signal
// but my plan to use multiple sockets failed, most likely do to a conflict
// between having two sockets on one port. I was not able to find a good solution
// in time to implement this.

//// Thread that watches for a kill signal and terminates the program
//// any signal on this socket should be interpreted as a kill signal
//void *watcher_handler(void *args) {
//	bool connected = false;
//	struct addrinfo *addrs;
//	struct addrinfo hints;
//	char id[BUFFER_SIZE];
//
//	memset(&hints, 0, sizeof(struct addrinfo));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_flags = 0;
//	hints.ai_protocol = 0;    
//
//	getaddrinfo(host, port, &hints, &addrs);
//
//	for (struct addrinfo *ad = addrs; ad != NULL && connected == false; ad = ad->ai_next) {
//		if (connect(watcher_socket_fd, ad->ai_addr, ad->ai_addrlen) == 0) {	// init watcher
//			encode(ID_WATCHER, id);
//			std::cout << send(watcher_socket_fd, id, BUFFER_SIZE, 0) << std::endl;
//			std::cout << errno << std::endl;
//			connected = true;
//			pthread_create(&watcher_thread_id, NULL, watcher_handler, NULL);
//			std::cout << "Making watcher" << std::endl;
//		} else { std::cout << "But it failed to connect at address " << ad->ai_addr << std::endl; }
//	}
//	std::cout << "Running watcher" << std::endl;
//	while (recv(watcher_socket_fd, NULL, 1, 0) > 0 ) {
//		//std::cout << errno << " " << std::endl;	// Ignore any input and spin until the server side shuts down
//	}
//	// then kill everything
//	kill(getpid(), 1);
//	return NULL;
//}
