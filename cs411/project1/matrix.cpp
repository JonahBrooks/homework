/*
 * Put the header here~
 *
 */

/*
 * TODO:
 *    Make a threaded version of this code
 *    in which the number of rows of the result matrix
 *    is divided evenly amongst the threads
 *
 */


#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/random.hpp>

/*
 * Global constants
 *
 */

int length; // Stores the length (and width) of the matrices

// matrices[0] = AB, [1] = A, [2] = A transpose, [3] = B, [4] = B transpose
double **matrix[5];

boost::mt19937 generator(std::time(0));

/* 
 * Function declarations
 *
 */

// Takes two vectors, returns the dot product of those two vectors
double dot(double *A, double *B, int size);
void seed_matrices();

int main(int argc, char *argv[]) {
	length = 1000;

	for (int m = 0; m < 5; ++m) {
		matrix[m] = (double**) malloc (sizeof(double*) * length);
		for (int i = 0; i < length; ++i) {
			matrix[m][i] = (double*) malloc (sizeof(double) * length);
		}	
	}	

	seed_matrices();

	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			matrix[0][i][j] = dot(matrix[1][i], matrix[4][j], length);
		}
	}
	
	// print out matrices
	std::cout << "~~ Matrix A ~~" << std::endl;
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			std::cout << matrix[1][i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "\n\n~~ Matrix B ~~" << std::endl;
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			std::cout << matrix[3][i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "\n\n~~ Matrix AB ~~" << std::endl;
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			std::cout << matrix[0][i][j] << " ";
		}
		std::cout << std::endl;
	}

	/* 
	 * Put code here~
	 */
	for (int m = 0; m < 5; ++m) {
		for (int i = 0; i < length; ++i) {
			free(matrix[m][i]);
		}	
		free(matrix[m]);
	}	

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

// Calculates the dot product of two vectors
double dot(double *A, double *B, int size) {

	double dot_product = 0;
	for (int i = 0; i < size; ++i) {
		dot_product += A[i] * B[i];
	}

	return dot_product;
}
