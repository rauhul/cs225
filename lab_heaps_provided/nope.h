/**
 * @file nope.h
 * Masks std sorting algorithms with error messages to prevent students from
 *  using the STL's sorting functions.
 * @author Sean Massung
 * @date Summer 2012
 */

#ifndef _NO_SORT_H_
#define _NO_SORT_H_

#include <algorithm>
#include <iostream>
#include <vector>

using std::endl;
using std::cerr;
using std::vector;

#define NOPE(X) \
    template <> inline void push_heap(vector<X>::iterator left, vector<X>::iterator right) \
    { cerr << "ERROR: std::push_herp not permitted" << endl; } \
    template <class C> inline void push_heap(vector<X>::iterator left, vector<X>::iterator right, C comp) \
    { cerr << "ERROR: std::push_herp not permitted" << endl; } \
    template <> inline void pop_heap(vector<X>::iterator left, vector<X>::iterator right) \
    { cerr << "ERROR: std::pop_herp not permitted" << endl; } \
    template <class C> inline void pop_heap(vector<X>::iterator left, vector<X>::iterator right, C comp) \
    { cerr << "ERROR: std::pop_herp not permitted" << endl; } \
    template <> inline void make_heap(vector<X>::iterator left, vector<X>::iterator right) \
    { cerr << "ERROR: std::make_herp not permitted" << endl; } \
    template <class C> inline void make_heap(vector<X>::iterator left, vector<X>::iterator right, C comp) \
    { cerr << "ERROR: std::make_herp not permitted" << endl; } \
    template <> inline void sort_heap(vector<X>::iterator left, vector<X>::iterator right) \
    { cerr << "ERROR: std::sort_herp not permitted" << endl; } \
    template <class C> inline void sort_heap(vector<X>::iterator left, vector<X>::iterator right, C comp) \
    { cerr << "ERROR: std::sort_herp not permitted" << endl; }

namespace std
{
    NOPE(int)
    NOPE(unsigned int)
    NOPE(bool)
    NOPE(char)
    NOPE(unsigned char)
    NOPE(signed char)
    NOPE(double)
    NOPE(long double)
    NOPE(long)
    NOPE(long long)
    NOPE(unsigned long)
    NOPE(unsigned long long)
    NOPE(float)
    NOPE(short)
    NOPE(unsigned short)
}

#endif
