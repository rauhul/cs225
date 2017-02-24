/**
 * @file fib.cpp
 * Contains Fibonacci-number generating functions both utilizing and not
 * utilizing memoization.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "fib.h"

#include <map>
#include <iostream>
using std::cout;
using std::endl;
using std::map;

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
unsigned long fib(unsigned long n) {
	if (n == 0) {
		return 0; 
	}
	if (n == 1) {
		return 1;
	}

	int prevprevNum = 0;
	int prevNum     = 1;
	int val;
	unsigned long iter = 2;
	while (n >= iter) {
		val = prevprevNum + prevNum;
		prevprevNum = prevNum;
		prevNum = val;
		iter++;
	}
    return prevNum;
}

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0. This version utilizes memoization.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
unsigned long memoized_fib(unsigned long n) {
	if (n == 0) {
		return 0; 
	}
	if (n == 1) {
		return 1;
	}
	map<unsigned long, unsigned long> m;
	m[0] = 0;
	m[1] = 1;
	unsigned long iter = 2;
	while (n >= iter) {
		m[iter] = m[iter-1] + m[iter-2];
		iter++;
	}
    return m[iter - 1];
}
