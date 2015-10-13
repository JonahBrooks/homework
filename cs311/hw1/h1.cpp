/*
 * Original Author: Jonah Brooks (brookjon)
 * File: h1.cpp
 * Created: 2012 January 18 14:04
 * Last Modified: 2012 January 23 18:56
 *
 * Program to copy an arbitrary file.
 *
 * Arguments are: [1] file_to_copy
 *                [2] new_file_name
 *
 *
 * I helped the following student:
 * Carol Fowler- helped with debugging/explaining how things worked as follows:
 *   Basics of manpages and tar.
 *   Timing functions, specifically clock()
 *   scope resolution, sepcifically which functions are part of std namespace
 *   debugging, mostly in buffer size and file permissions
 *
 * I collaborated with the following student:
 * Savannah Van Beek - Discussed various programming concepts
 *                     Exchanged coding options and strategies, ie ctime vs sys/times.h
 *                     Worked together to install/configure vim and git
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <math.h>

int main(int argc, char *argv[]) {

	int fd;       // File Descriptor for reading
	int tmp_fd;   // File Descriptor to see if file already exists
	int out_fd;   // File Descriptor for writing

	int block_size = 4;   // Changed from 1 to 4 for usability; 1 and 2 are really slow

	int number_read;
	bool error = false;

	char *buffer; // Buffer to be dynamically allocated in each loop

	double time_start = 0.0;

	/* Check to see if the file already exists */
	tmp_fd = open(argv[2], O_RDONLY);
	close(tmp_fd);

	/* Validate user input */
	if (argc != 3) {
		std::cout << "Error, must supply two arguments: file_to_copy, new_file_name" << std::endl;
		error = true;
	} else if (tmp_fd != -1) {
		error = true;
		std::string input = "n"; 
		std::cout << "Error, file already exists!" << std::endl;
		std::cout << "Overwrite file and continue? (y/n): ";
		std::cin >> input;
		if (input.at(0) == 'y' || input.at(0) == 'Y') {
			error = false;
		} else {
			error = true;
			std::cout << "Canceling copy process..." << std::endl;
		}
	} 
	
	/* Loop through and copy file using various block sizes */
	fd = open(argv[1], O_RDONLY);
	while (error == false && block_size <= pow(2,29)) {
		buffer = (char *) std::malloc(block_size);

		out_fd = creat(argv[2], S_IRWXO | S_IRWXU);
		time_start = ((double)std::clock()) / CLOCKS_PER_SEC; 
		
		number_read = read(fd, buffer, block_size);
		while (number_read > 0) {
			write(out_fd, buffer, number_read);
			number_read = read(fd, buffer, block_size);
		}

		close(out_fd);
		
		/* Output the time taken for use in gnuplot */
		std::cout << ( (double)log(block_size) / (double)log(2) ) << " ";
		std::cout << ( ((double)std::clock()) / CLOCKS_PER_SEC - time_start);
		std::cout << std::endl;

		/* Reset files and prepare to copy again with new block size */
		lseek(fd, 0, SEEK_SET);
		block_size *= 2;
		std::free(buffer);
	}
	
	close(fd);
	return 0;
}
