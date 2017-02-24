#ifndef DSETS_H
#define DSETS_H

// c-style includes
#include <stdio.h>
#include <stdlib.h>

// c++ style includes
#include <iostream>
#include <vector>

using namespace std;

class DisjointSets {
    public:
    	//Creates n unconnected root nodes at the end of the vector
		void addelements(int num);
		//This function should compress paths and works as described in lecture
		int find(int elem);
		//This function should be implemented as union-by-size
		void setunion(int a, int b);
	private:
		//Storage for the DisjointSets
		vector<int> elements;
		//Helper method to check index validity before use
		bool valid(int index);
		//Helper method to check if the elem at a given index is a root
		bool isRoot(int index);
		//Helper method to recursive find the root of a given index and compress paths
		int findRecur(int index);
		//Helper method to print out the elements vector used to assist with debugging 
		void printElements();
		//Helper method to print out a single element from elements used to assist with debugging 
		void printElement(int index);
};
#endif // DSETS_H