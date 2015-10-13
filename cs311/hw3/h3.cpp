/*
 * Original Author: Jonah Brooks (brookjon)
 * File: h4.cpp
 * Created: 2012 February 11 2:16
 * Last Modified: 2012 February 23 16:46
 *
 * Calculates all primes under 2^23
 *
 * Discussed basic concepts with Savannah Van Beek:
 *    Discussed algorithm options
 *    Helped with debugging, primarilly segfaults
 *
 */

// WARNING! Never attempt to lock verified_mutex from within a lock on a byte section

#define _POSIX_SOURCE

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <pthread.h>
#include <ctime>
#include <sys/time.h>
#include <cstring>
#include <vector>
#include <string>

#define NUMBER_OF_THREADS 8 
#define BYTES_PER_SECTION 1024

// GLOBAL VARIABLES
const unsigned long long NUMBER_OF_BITS = std::pow(2.0, 32.0); 
const unsigned long long NUMBER_OF_BYTES = NUMBER_OF_BITS / 8;
unsigned long long MAX_TO_CHECK = (unsigned long long)sqrt(NUMBER_OF_BITS);

// number of lockable sections
const unsigned long long NUMBER_OF_SECTIONS = NUMBER_OF_BYTES / BYTES_PER_SECTION;


unsigned long long verified = 0; // stores the most recently verified prime
pthread_mutex_t verified_mutex = PTHREAD_MUTEX_INITIALIZER;
std::vector<pthread_mutex_t> section_mutexes(NUMBER_OF_SECTIONS);

char BIT_MASKS[8] = {1, 2, 4, 8, 16, 32, 64, 128};
// END GLOBAL VARIABLES


typedef char byte_t;

struct thread_info {
	std::vector<byte_t> *bytes;
	int id;
};

void *thread_handler(void *arg_struct);


// Timing functions
double to_sec(struct timeval tm);
double sec_since(struct timeval tm);


// Bitmap functions
unsigned long long section_of_number(unsigned long long number);

bool check_bit(byte_t byte, int offset);
void set_bit(byte_t* byte, int offset);
void unset_bit(byte_t* byte, int offset);

bool check_number(std::vector<byte_t> *bytes, unsigned long long number);
void set_number(std::vector<byte_t> *bytes, unsigned long long number);
void unset_number(std::vector<byte_t> *bytes, unsigned long long number);


int main(int argc, char **args) {
    // 1s will be not prime, 0s will be prime
	// use set to mark non-primes

    struct timeval start;
    gettimeofday(&start, NULL);

	// initialize all even numbers to be not-prime
	byte_t preset = 0;
	preset |= 1 << 0;
	preset |= 1 << 2;
	preset |= 1 << 4;
	preset |= 1 << 6;

	std::vector<byte_t> bytes(NUMBER_OF_BYTES, preset);
	unset_number(&bytes, 2); // 2 is prime
	set_number(&bytes, 1);	// 1 is not prime
	verified = 2;
	
	for (unsigned long long i = 0; i < NUMBER_OF_SECTIONS; ++i) {
		pthread_mutex_init(&section_mutexes[i], NULL);
	}
	
	struct thread_info *info;
	info  = (struct thread_info*) malloc (NUMBER_OF_THREADS * sizeof(struct thread_info));
	pthread_t *threads = (pthread_t*) malloc (NUMBER_OF_THREADS * sizeof(pthread_t));

    struct timeval post_init;
    gettimeofday(&post_init, NULL);

	
	// fill each thread's info struct then create that thread
	for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
		info[i].bytes = &bytes;	
		info[i].id = i;
	
		pthread_create(&threads[i], NULL, thread_handler, &info[i]);
	}
	

	for (unsigned long long i = 0; i < NUMBER_OF_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}
	free(threads);
	free(info);

	for (unsigned long long i = 0; i < NUMBER_OF_SECTIONS; ++i) {
		pthread_mutex_destroy(&section_mutexes[i]);
	}

	std::cout << sec_since(start) << std::endl;
	std::cout << sec_since(post_init) << std::endl;
	std::cout << "Counting number of primes found" << std::endl;
	int count = 0;
	for (unsigned long long i = 0; i < NUMBER_OF_BITS; ++i) {
		if ( check_number(&bytes, i) == 0 ) {
			//toggle this on to print the list of primes found
			//std::cout << i << std::endl; 
			++count;
		}
	} 
	std::cout << count << " primes found." << std::endl;
	std::cout << NUMBER_OF_THREADS << " threads and ";
	std::cout << NUMBER_OF_SECTIONS << " mutex sections." << std::endl;
	return 0;
}


// Function that is called when a new thread is created
// Flags bits as not prime for every multiple of primes
// Uses verified to keep track of which prime to process next
// and updates that variable when it starts the flaging process.
void *thread_handler(void *arg_struct) {
	struct thread_info info = *((struct thread_info*) arg_struct);
	bool finished = false;
	unsigned long long current = 0;

	while (finished == false) {
		pthread_mutex_lock(&verified_mutex);
		// find next possible prime
		current = verified + 1;
		while (current < MAX_TO_CHECK
		       && check_number(info.bytes, current) == 1) {
			++current;
		}
		if (current > MAX_TO_CHECK) {
			finished = true;
		}

		// set verified to equal that prime
		verified = current;

		// unlock verified
		pthread_mutex_unlock(&verified_mutex);	


		// Set all multiples of current prime as non-primes
		// skip every even multiple because it will have been counted during prime = 2
		// start at current*current, since anything smaller than that will be a composite
		// composed of this number and one (or more) of the primes already processed
		for (unsigned long long i = current*current; i < NUMBER_OF_BITS && finished == false; i += current*2) {
			// find the next multiple of current

			// check to see if that multiple is already set as non-prime
			if (check_number(info.bytes, i) == 0) { // if we thought it was a prime
				// then, if not, lock that section's mutex
				pthread_mutex_lock(&(section_mutexes[section_of_number(i)]));
				// set that multiple as non-prime
				set_number(info.bytes, i);
				// unlock that section's mutex
				pthread_mutex_unlock(&(section_mutexes[section_of_number(i)]));	
				// repeat
			}
		}
		
	}
	return NULL;
}


// returns the number of seconds since timeval was set
double sec_since(struct timeval tm) {
    struct timeval now;
    gettimeofday(&now, NULL);

    return to_sec(now) - to_sec(tm);
}


// converts the input timeval to seconds
double to_sec(struct timeval tm) {
    return tm.tv_sec + (double)tm.tv_usec / 1000000.0;
}


// Returns the section number that contains the specified number
unsigned long long section_of_number(unsigned long long number) {
	return number / (8 * BYTES_PER_SECTION); 
}


// looks up number in bytes array, checking for previously set
// prime status. Returns the value of the bit representing number
bool check_number(std::vector<byte_t> *bytes, unsigned long long number) {
	unsigned long long byte_num = number / 8;
	unsigned long long bit_num = number % 8;

	bool is_prime = check_bit((*bytes)[byte_num], bit_num);
	return is_prime;
}


// sets number as non-prime in the bitmap
void set_number(std::vector<byte_t> *bytes, unsigned long long number) {
	unsigned long long byte_num = number / 8;
	unsigned long long bit_num = number % 8;

	set_bit(&(*bytes)[byte_num], bit_num);
}


// sets number as prime in the bitmap
void unset_number(std::vector<byte_t> *bytes, unsigned long long number) {

	unsigned long long byte_num = number / 8;
	unsigned long long bit_num = number % 8;

	unset_bit(&(*bytes)[byte_num], bit_num);
}


// checks to see if the specified bit is set to 1
// returns true if so
bool check_bit(byte_t byte, int offset) {

	return (bool)(byte & BIT_MASKS[offset]);
}


// sets the specified bit to 1
void set_bit(byte_t* byte, int offset) {
	*byte |= BIT_MASKS[offset];
}


// sets the specified bit to 0
void unset_bit(byte_t* byte, int offset) {
	*byte &= ~BIT_MASKS[offset];
}

