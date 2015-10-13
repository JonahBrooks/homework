Jonah Brooks
CS325 Programming Assignment 1
01/29/2012

I worked with Savannah Van Beek, but we wrote all our own code.

fib1:
	Implements the first (recursive) algorithm for finding Fibonacci Numbers.

fib2:
	Implenents the second (iterative) algorith for finding Fibonacci Numbers.

fib3:
	Uses a Stack (or... simulation thereof) instead of an array to implement the iterative algorithm. This is because, due to story the Fibonacci Numbers in an array, fib2 ends up at (near) constant time. This function is closer to what the second algorithm should produce.

fibGraph:
	Graphs the time taken vs number of Fibonacci Numbers computer for all three functions in normal and semilog scale.

fibChart:
	Charts the first 100 Fibonacci Numbers against the estimated Fibonacci Numbers in both normal and semilog scale.


Comments on the graphs:

	I noticed that fib1 seems to run in exponential time O(n^2) according to the graph, which is also what it should run according to runtime analysis. fib2 seems to run in constant time, since I'm filling an entire array and Matlab is (seemingly) storing that between calls. fib3, which does not use a full array,  appears to run in linear time, which is what I was expecting from the algorithm's runtime analysis.

	Ploting this on a semilog scale seems to agree with the above observations, turning the fib1 data into a linear (and therefore originally exponential) graph.


Question 3:

	Given that the time function for fib1 seems to be t(n) = 1.366*10^-5 * 1.65^n, I can estimate that fib1 could calculate the first 37 Fibonacci Numbers in a half hour.

	fib2, on the other hand, seems to be nearly linear, and therefore would likely overflow my programs memory in under a half hour...

	fib3, which functions how fib2 is logically supposed to, seems to have a time function of around t(n) = 3.5*10^-6 * 1.06^n, it seems fib3 could calculate the 344th Fibonacci Number in a half hour.

	Needless to say, fib2 or fib3 seems to be the way to go for large values of n.
