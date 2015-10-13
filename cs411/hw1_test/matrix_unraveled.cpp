/*
 * Original Author: Jonah Brooks (brookjon)
 * File: matrix_unraveled.cpp
 * Created: 2012 April 14 by brookjon
 * Last Modified: 2012 April 17 by brookjon
 * 
 * This file contains the serial implementation of matrix multiplication. 
 * It makes use of a command line flag parser file that I wrote expressly
 * for this purpose.  
 *
 * Design decisions:
 *   I chose to write this code in a very modular way, making use of many functions
 *     and headers. This was done to make it easily adaptable when adding threads.
 */	

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <boost/random.hpp>
#include <string>
#include "switch_parser.hpp"


// Global variables

int length; // Stores the length (and width) of the matrices

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

int main(int argc, char *argv[]) {

	// Using my header for flag parsing
	switch_parser::switcher arg_flags(argc, argv);

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

	// Calculate results
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			matrix[0][i][j] = dot(matrix[1][i], matrix[4][j], length);
		}
	}
	
	// Print out matrices
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
