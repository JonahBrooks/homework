/*
 * Original Author: Jonah Brooks (brookjon)
 * File: matrix_threaded.cpp
 * Created: 2012 April 14 by brookjon
 * Last Modified: 2012 April 17 by brookjon
 * 
 * This file contains the threaded implementation of matrix multiplication.
 * It makes use of a command line flag parser file that I wrote expressly
 * for this purpose.  
 * The boost libraries were used for threading since I already have experience
 * with using pthreads.
 *
 * Discussed basic algorithm decisions with Savannah Van Beek,
 * as well as sharing links to boost library resources.
 *
 */	

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/random.hpp>
#include <string>
#include "switch_parser.hpp"


// Global variables

int length; // Stores the length (and width) of the matrices
int number_of_threads = 8;
boost::thread *threads;

// matrices[0] = AB, [1] = A, [2] = A transpose, [3] = B, [4] = B transpose
double **matrix[5];

// File streams for the three matrices
std::ifstream a_in;
std::ifstream b_in;
std::ofstream ab_out;

boost::mt19937 generator(std::time(0));


// Function headers

// Takes two vectors, returns the dot product of those two vectors
double dot(double *A, double *B, int size);
void seed_matrices();
void fill_matrices();
void thread_handler(int id);

int main(int argc, char *argv[]) {

	// Using my header for flag parsing
	switch_parser::switcher arg_flags(argc, argv);
	
	number_of_threads = arg_flags.find_long("t", 8);
	if (number_of_threads < 2) {
		number_of_threads = 2;
		std::cout << "Too few threads: Increasing to the minimum 2" << std::endl;
	} else if (number_of_threads > 300) {
		number_of_threads = 300;
		std::cout << "Too many threads: Decreasing to the maximum 300" << std::endl;
	}	

	length = arg_flags.find_long("l", 10);
	
	arg_flags.find_ifstream("a", "", a_in);
	arg_flags.find_ifstream("b", "", b_in);
	arg_flags.find_new_ofstream("o", "result_matrix.txt", ab_out);

	if (!ab_out.is_open()) {
		std::cout << "Output file already exists: Outputting to "
		          << "result_matrix.txt (overwritting if needed)\n";
		ab_out.open("result_matrix.txt");
	}

	// Malloc the matrices
	for (int m = 0; m < 5; ++m) {
		matrix[m] = (double**) malloc (sizeof(double*) * length);
		for (int i = 0; i < length; ++i) {
			matrix[m][i] = (double*) malloc (sizeof(double) * length);
		}	
	}
	
	// Fill or seed matrices
	if (a_in.is_open() && b_in.is_open()) {
		fill_matrices();
	} else {	
		seed_matrices();
	}

	std::cout << "Input matrices filled: Calculating result matrix" << std::endl;

	// Malloc the threads
	threads = (boost::thread*) malloc (sizeof(boost::thread) * number_of_threads);

	// Calculate results
	for (int i = 0; i < number_of_threads; ++i) {
		threads[i] = boost::thread(thread_handler, i);	
	}
	
	// Join the threads
	for (int i = 0; i < number_of_threads; ++i) {
		threads[i].join();
	}
	
	// Free the threads
	free(threads);
	
	// Print out matricesi
	std::cout << "Result matrix calculated and stored in the output file "
	          << "(result_matrix.txt by default)" << std::endl << std::endl;
	std::cout << "~ Matrix A ~" << std::endl;	
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			ab_out << matrix[0][i][j] << " ";
			std::cout << matrix[1][i][j] << " ";
		}
		ab_out << std::endl;
		std::cout << std::endl;
	}
	std::cout << "\n\n~ Matrix B ~" << std::endl;
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			std::cout << matrix[3][i][j] << " ";
		}
		std::cout << std::endl;
	}
	

	// Free the matrices
	for (int m = 0; m < 5; ++m) {
		for (int i = 0; i < length; ++i) {
			free(matrix[m][i]);
		}	
		free(matrix[m]);
	}	

	a_in.close();
	b_in.close();
	ab_out.close();

	return 0;
}


// Each thread will handle length/number_of_threads rows of the result matrix
void thread_handler(int id) {
	
	int division = ceil( (double) length / (double) number_of_threads );

	for (int i = division * id; (i < length) && (i < (division * id + division)); ++i) {
		for (int j = 0; j < length; ++j) {
			matrix[0][i][j] = dot(matrix[1][i], matrix[4][j], length);
		}
	}

}


// Seeds both matrices with random values
void seed_matrices() {
	boost::uniform_real<double> real_dist(-1.0,1.0);
	double random_number = real_dist(generator);
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			random_number = real_dist(generator);
			matrix[1][i][j] = random_number;
			matrix[2][j][i] = random_number;
			random_number = real_dist(generator);
			matrix[3][i][j] = random_number;
			matrix[4][j][i] = random_number;
		}
	}

}


void fill_matrices() {
	double a_next;
	double b_next;
	int count = length*length;
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length && (a_in >> a_next) && (b_in >> b_next); ++j) {
            count--;
            matrix[1][i][j] = a_next;
			matrix[2][j][i] = a_next;
			matrix[3][i][j] = b_next;
			matrix[4][j][i] = b_next;
        }
    }

	if (count != 0) {
		std::cout << "Error: input matrix does not match size given" << std::endl;
		exit(EXIT_FAILURE);
	}	

}


// Calculates the dot product of two vectors
double dot(double *A, double *B, int size) {

	double dot_product = 0;
	for (int i = 0; i < size; ++i) {
		dot_product += A[i] * B[i];
	}

	return dot_product;
}
