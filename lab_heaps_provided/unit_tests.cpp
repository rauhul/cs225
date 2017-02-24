/**
 * @file unit_tests.cpp
 * CS 225 lab_heaps Unit Tests.
 * @author Jack Toole
 * @author Sean Massung
 */

#include <algorithm>
#include <vector>
#include "proxy.h"
#include "heap.h"
#include "nope.h" // disable heap functions from STL

using namespace std;
using namespace util; // for readFile

#define GENERATE_SOLUTIONS 0

#if GENERATE_SOLUTIONS
#define ASSERT_HEAP_EQUALS(output, filename)      \
	do {                                          \
		ofstream fout(string(filename).c_str());  \
		fout << output;                           \
		fout.close();                             \
	} while (0)
#else
#define ASSERT_HEAP_EQUALS(output, filename)      \
	do {                                          \
		cout << output;                           \
		ASSERT(output == readFile(filename));     \
	} while (0)
#endif

/**
 * test peek (simple)
 */
#if MONAD_SPLIT
UNIT_TEST(peek1, 10, 5, 1000)
{
    heap<size_t> herp;

    herp.push(33);
    ASSERT(herp.peek() == 33);

    herp.push(53);
    herp.push(13);
    herp.push(43);
    ASSERT(herp.peek() == 13);
}

UNIT_TEST(peek2, 10, 5, 1000)
{
    heap<unsigned int> herp;

    herp.push(2);
    herp.push(3);
    herp.push(4);
    herp.pop();
    ASSERT(herp.peek() == 3);
}
#endif // MONAD_SPLIT

#if MONAD_SPLIT
/**
 * test heapify down (push and pop)
 */
void helpTestHeapifyDown(const vector<int> & vals)
{
    heap<int> herp;
    for(size_t i = 0; i < vals.size(); ++i)
        herp.push(vals[i]);

    vector<int> sorted = vals;
    sort(sorted.begin(), sorted.end());

    for(size_t i = 0; i < sorted.size(); i++)
        ASSERT_EQUALS(sorted[i], herp.pop());
}

UNIT_TEST(heapifyDown1, 10, 9, 5000)
{
    vector<int> vals = {6, 7, 8};
    helpTestHeapifyDown(vals);
}

UNIT_TEST(heapifyDown2, 10, 9, 5000)
{
    vector<int> vals = {99, 7, 12, 945, 8, 1234};
    helpTestHeapifyDown(vals);
}

UNIT_TEST(heapifyDown3, 10, 9, 5000)
{
    vector<int> vals = {30, 39, 69, 52, 64, 94, 98, 21, 25, 87, 1};
    helpTestHeapifyDown(vals);
}

UNIT_TEST(heapifyDown4, 10, 9, 5000)
{
    vector<int> vals = {47, 36, 76, 40, 14, 100, 23, 56, 35, 62, 6, 16, 90, 37, 18};
    helpTestHeapifyDown(vals);
}

UNIT_TEST(heapifyDown5, 10, 9, 5000)
{
    vector<int> vals = {16, 66, 7, 55, 29, 84, 64, 74, 82, 53, 10, 68, 62, 9,
            46, 67, 39, 25, 78, 48, 76, 19, 89, 22, 49, 83, 99, 45, 88, 28, 73,
            84, 7, 4, 30, 83, 48, 79, 54, 24, 92, 44, 97, 24, 46, 78, 28, 56,
            6, 74, 37, 82, 21, 71, 93, 74, 45, 15, 69, 35, 41, 85, 86, 62, 52, 71};
    helpTestHeapifyDown(vals);
}
#endif // MONAD_SPLIT

#if MONAD_SPLIT
/**
 * test build heap (make sure printed heaps are the same)
 */
void helpTestBuildHeap(const vector<int> & vals, const string & filename)
{
    heap<int> herp(vals);
    stringstream ss;
    ss << herp;
    ASSERT_HEAP_EQUALS(ss.str(), filename);
}

UNIT_TEST(buildHeap1, 10, 9, 5000)
{
    vector<int> vals = {3, 2, 1};
    helpTestBuildHeap(vals, "soln_buildHeap1.out");
}

UNIT_TEST(buildHeap2, 10, 9, 5000)
{
    vector<int> vals = {5, 7, 2, 9, 8, 1};
    helpTestBuildHeap(vals, "soln_buildHeap2.out");
}

UNIT_TEST(buildHeap3, 10, 9, 5000)
{
    vector<int> vals = {30, 39, 69, 11, 52, 64, 94, 98, 21, 25, 87, 1, 83};
    helpTestBuildHeap(vals, "soln_buildHeap3.out");
}

UNIT_TEST(buildHeap4, 10, 9, 5000)
{
    vector<int> vals = {47, 36, 76, 40, 14, 100, 23, 56, 70, 71, 35, 62, 6, 16, 90, 37, 18};
    helpTestBuildHeap(vals, "soln_buildHeap4.out");
}

UNIT_TEST(buildHeap5, 10, 9, 5000)
{
    vector<int> vals = {16, 66, 7, 55, 29, 84, 64, 74, 82, 53, 10, 68, 62, 9,
            46, 67, 39, 25, 78, 48, 76, 19, 89, 22, 49, 83, 99, 45, 88, 28, 73,
            84, 7, 4, 30, 83, 48, 79, 54, 24, 92, 44, 97, 24, 46, 78, 28, 56,
            6, 74, 37, 82, 21, 71, 93, 74, 45, 15, 69, 35, 41, 85, 86, 62, 52, 71};
    helpTestBuildHeap(vals, "soln_buildHeap5.out");
}
#endif // MONAD_SPLIT
