/*
 * Original Author: Jonah Brooks (brookjon)
 * File: h2.cpp
 * Created: 2012 January 30 17:53
 * Last Modified: 2012 February 7 17:20
 *
 * Program that takes in text through stdin, sends text to various sort
 * processes in a round-robin style, then merges all words back together
 * before printing them in alphabetical order in lower-case.
 *
 * Arguments are: [1] number of sort processes to create
 *
 * I/O: Takes input from stdin, returns output from stdout
 *    It is recommended to call this program using:
 *       cat file_name | program_path [numer_of_children] > output_file
 *
 * I helped the following student:
 * Carol Fowler- Basic help with programming concepts and debugging.
 *   explained malloc, and answered questions involving pipes and forking
 *   debugging, mostly involving malloc
 *
 * I collaborated with the following student:
 * Savannah Van Beek - Help debugging and discussed various programming concepts
 *   we helped each other track down a few key bugs, 
 *   primarily fork bombs, segfaults, and pipe closures
 *
 */


#define _POSIX_SOURCE

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/wait.h>
#include <cerrno>
#include <vector>

#define CHILD_MAX 250

// GLOBAL VARIABLES
int NUMBER_OF_CHILDREN = 42;
int NUMBER_OF_PIPES = NUMBER_OF_CHILDREN * 2;
// END GLOBAL VARIABLES


void parser(int **pipes);
void sorter(int index, int **pipes);
void suppressor(int index, int **pipes);
int index_of_smallest(std::vector<std::string> words, int size);
void normalize(char *str);


int main(int argc, char *argv[]){

	if (argc == 2) {
		NUMBER_OF_CHILDREN = atoi(argv[1]);
		if (NUMBER_OF_CHILDREN > CHILD_MAX) {
			NUMBER_OF_CHILDREN = CHILD_MAX;
		}
		NUMBER_OF_PIPES = NUMBER_OF_CHILDREN * 2;
	}

	// Create pipes. Pipes 0-NUMBER_OF_CHILDREN will be from the parser to the sorters
	// Pipes NUMBER_OF_CHILDREN to NUMBER_OF_PIPES 
	// will be from the sorters to the suppressor
	// (NUMBER_OF_CHILDREN + child identifier index) 
	int **pipe_array = (int**) malloc ( NUMBER_OF_PIPES * sizeof(int**) );
	for (int i = 0; i < NUMBER_OF_PIPES; ++i) {
		pipe_array[i] = (int*) malloc (2 * sizeof(int*));
		pipe(pipe_array[i]); 
		fdopen(pipe_array[i][0], "r");
		fdopen(pipe_array[i][1], "w");
	}

	bool is_parent = false;	
	// Forks many childrens
	for (int i = 0; i < NUMBER_OF_CHILDREN + 1; ++i) {
		is_parent = false;
		// i will be used to identify each child uniquely
		switch (fork()){
			case -1:
				std::cout << "ERROR!" << std::endl;
				break;
			case 0:
				// I'm... I'm a clone? ;_;
				if (i == NUMBER_OF_CHILDREN) {
					suppressor(i, pipe_array);
				} else {
					sorter(i, pipe_array);
				}
				i = NUMBER_OF_CHILDREN + 2; // >_< I know I know. Population crisis = serious
				break;
			default: 
				is_parent = true;
		}
	}

	if (is_parent == true) {
		parser(pipe_array);
	}
	
	// Free the malloc'ed array in every process
	// Exec will replace the heap, so I need not free the wild pointers	
	for (int i = 0; i < NUMBER_OF_PIPES; ++i) {
		free(pipe_array[i]);
	}
	free(pipe_array);
	
	return 0;
}


// Parses the words from standard in, normalizes them, sends them 
// through the streams in round-robin style to be sorted
void parser(int **pipe_array){

	FILE **fstreams = (FILE**) malloc (NUMBER_OF_CHILDREN * sizeof(FILE**));
	
	for (int i = 0; i < NUMBER_OF_CHILDREN; ++i) {
		close(pipe_array[i][0]);
		close(pipe_array[i + NUMBER_OF_CHILDREN][0]);
		close(pipe_array[i + NUMBER_OF_CHILDREN][1]);
		fstreams[i] = fdopen(pipe_array[i][1], "w");
	}

	// Parser!!!!
	std::string moo;
	int round_robin = 0;
	char cow[1024];
	while ( std::cin >> moo ) {
		moo += '\n'; // Delimiter for the sort function
		strcpy (cow, moo.c_str());
		normalize(cow);
		fputs(cow, fstreams[round_robin]);
		round_robin = (round_robin + 1) % NUMBER_OF_CHILDREN;
	}

	// close and free the file descriptors
	for (int i = 0; i < NUMBER_OF_CHILDREN; ++i) {
		fclose(fstreams[i]);
	}
	free(fstreams);

	// wait for the children to die
	for (int i = 0; i < NUMBER_OF_CHILDREN + 1; ++i) {
		wait(NULL);
	}
}


// Sets up and execs to the sort function for sorting each stream
void sorter(int index, int **pipe_array){

	for (int i = 0; i < NUMBER_OF_CHILDREN; ++i) {
		if (i != index) {	// don't close either of this child's pipes
			close(pipe_array[i][0]); // But close everything else
			close(pipe_array[i][1]);
			close(pipe_array[i + NUMBER_OF_CHILDREN][0]);
			close(pipe_array[i + NUMBER_OF_CHILDREN][1]);
		} else {
			close(pipe_array[i][1]); // close write end of parser -> sorter pipe
			close(pipe_array[i + NUMBER_OF_CHILDREN][0]); // sorter -> suppressor read
		}
	}

	close(0);
	dup(pipe_array[index][0]);
	close(1);
	dup(pipe_array[index + NUMBER_OF_CHILDREN][1]);

	if (execl("/bin/sort", "sort", NULL) == -1) {
		if (execl("/usr/bin/sort", "sort", NULL) == -1) { // try this path instead
			std::cout << "ERROR! ERROR! EXECL HAS FAILED! ERRNO IS ";
			std::cout << errno << " " << strerror(errno) << std::endl;
		}
	}
}


// Pulls words out of the pipes one at a time, printing
// all the words without duplicates in alphabetical order
void suppressor(int index, int **pipe_array){
	// I can pull the words out one at a time and place them into an array
	// Each time I find a duplicate, I ignore it
	// After I get one word from every pipe, I can send off the "smallest" word
	// then pull another word from the pipe that provided that word
	// place the new word into the array, output the smallest, repeat

	FILE **fstreams = (FILE**) malloc (NUMBER_OF_CHILDREN * sizeof(FILE**));
	char buffer[1024];
	std::string last_word = "";
	std::string current_word;
	std::vector<std::string> temp_sort(NUMBER_OF_CHILDREN);
	int min; // Index in temp_sort of the smallest element

	for (int i = 0; i < NUMBER_OF_CHILDREN; ++i) { 
		close(pipe_array[i][0]); // Close all read
		close(pipe_array[i][1]); // and write ends of parser -> sorter pipes
		close(pipe_array[i + NUMBER_OF_CHILDREN][1]); // close write ends of pipes
		fstreams[i] = fdopen(pipe_array[i + NUMBER_OF_CHILDREN][0], "r"); // and open all the read ends
	}


	// Fill the array with one word from each pipe
	for (int i = 0; i < NUMBER_OF_CHILDREN; ++i) {
		if (fgets(buffer, 1024, fstreams[i])) {
			temp_sort[i] = buffer;
		} else {
			temp_sort[i] = ""; // This pipe is now empty
		}
	}

	// Output and refill the smallest element in the array
	min = index_of_smallest(temp_sort, NUMBER_OF_CHILDREN);
	while (min != -1) {
		current_word = temp_sort[min];
		if (current_word.compare(last_word) != 0) {
			std::cout << current_word;
			last_word = current_word;
		}
		if (fgets(buffer, 1024, fstreams[min])) {
			temp_sort[min] = buffer;
		} else { 
			temp_sort[min] = "";
		}
		min = index_of_smallest(temp_sort, NUMBER_OF_CHILDREN);
	}
		

	free(fstreams);
}


// Returns the index in vector words of the smallest element
// or -1 if all elements are empty
int index_of_smallest(std::vector<std::string> words, int size) {
	int min = 0;
	bool all_empty = true;

	for (int i = 0; i < size; ++i) {
		if (words[i].empty() == false) {
			all_empty = false;
			if ( (words[min].empty() == true) || (words[i].compare(words[min]) < 0)) {
				min = i;
			}
		} 
	}
	
	if (all_empty == true) {
		min = -1;
	}

	return min;
}


// Removes punctiation (replacing it with newlines), and makes everything lowercase
// Any non-standard characters will be replaced like punctiation, including accent marks
void normalize(char *str) {
	char *cur = str;
	while (*cur != '\0') {
		*cur = (char) tolower((int) *cur);
		if ((*cur < 'a' || *cur > 'z') 
		    && (*cur != '\'') 
		    && (*cur != '\n') 
		    && (*cur != '-') 
		    ){
			
			*cur = '\n';
		}
		cur++;
	}
}
		
