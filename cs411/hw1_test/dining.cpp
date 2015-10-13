/*
 * Original Author: Jonah Brooks (brookjon)
 * File: dining.cpp
 * Created: 2012 April 14 by brookjon
 * Last Modified: 2012 April 17 by brookjon
 *
 * This file contains my implementation of the Dining Philosophers problem.
 * In it I use the boost thread library to simulate the philosophers as threads
 * and the chopstick pile as a mutex. 
 *
 * Discussed basic algorithm decisions with Savannah Van Beek,
 * as well as sharing links to boost library resources.
 *
 */	


#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#define MAX_CHOPSTICKS 5
#define MAX_PHILOSOPHERS 5
#define MAX_DISHES 10
#define EAT_DELAY_MILLI 200
#define THINK_DELAY_MILLI 100

// Global variables
boost::mutex chopstick_pile; // Lock for checking / accessing the chopstick pile
int chopsticks_ready = MAX_CHOPSTICKS;

boost::posix_time::time_duration eating_delay;
boost::posix_time::time_duration thinking_delay;

boost::thread philosopher[MAX_PHILOSOPHERS];
int dishes_remaining[MAX_PHILOSOPHERS];

// Function headers
void dine(int id);

int main(int argc, char* argv[]) {

	eating_delay = boost::posix_time::milliseconds(EAT_DELAY_MILLI);
	thinking_delay = boost::posix_time::milliseconds(THINK_DELAY_MILLI);

	for (int i = 0; i < MAX_PHILOSOPHERS; ++i) {
		dishes_remaining[i] = MAX_DISHES;
		philosopher[i] = boost::thread(dine, i);
	}
	
	for (int i = 0; i < MAX_PHILOSOPHERS; ++i) {
		philosopher[i].join();
	}

	return 0;

}


void dine(int id) {
	bool has_chopsticks = false;
	
	while (dishes_remaining[id] > 0) {
		chopstick_pile.lock(); // lock mutex
		if (chopsticks_ready >= 2) {
			chopsticks_ready -= 2;
			has_chopsticks = true;
		}
		chopstick_pile.unlock(); // release mutex
		if (has_chopsticks == true) {
			// eat
			boost::this_thread::sleep(eating_delay);
			--dishes_remaining[id];

			chopstick_pile.lock(); // lock mutex
			std::cout << "Philosopher " << id << " just finished plate number " 
			          << (MAX_DISHES - dishes_remaining[id]) << std::endl;
			chopsticks_ready += 2;
			has_chopsticks = false;
			chopstick_pile.unlock(); // release mutex
		} 
		// then, regardless, think
		boost::this_thread::sleep(thinking_delay);
	}
}
