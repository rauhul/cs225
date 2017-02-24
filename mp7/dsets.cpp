#include "dsets.h"
using namespace std;

//Creates n unconnected root nodes at the end of the vector
void DisjointSets::addelements(int num) {
	for (int i = 0; i < num; i++) {
		elements.push_back(-1);
	}
}

//This function should compress paths and works as described in lecture
int DisjointSets::find(int elem) {
	if (!valid(elem))
		return -1;
	return findRecur(elem);
}

//This function should be implemented as union-by-size
void DisjointSets::setunion(int a, int b) {
	if (!valid(a) || !valid(b))
		return;
	int rootA = find(a);
	int rootB = find(b);
	if (rootA == rootB)
		return;
	if (elements[rootA] <= elements[rootB]) { //More elements in treeA
		elements[rootA] = elements[rootA] + elements[rootB];
		elements[rootB] = rootA;
	} else { 								  //Move elements in treeB
		elements[rootB] = elements[rootA] + elements[rootB];
		elements[rootA] = rootB;
	}
}

//Helper method to check index validity before use
bool DisjointSets::valid(int index) {
	return index >= 0 && index < int(elements.size());
}

//Helper method to check if the elem at a given index is a root
bool DisjointSets::isRoot(int index) {
	return elements[index] < 0;
}

//Helper Method to recursive find the root of a given index and compress paths
int DisjointSets::findRecur(int index) {
	if (isRoot(index))
		return index;
	int root = findRecur(elements[index]);
	elements[index] = root;
	return root;
}

//Helper method to print out the elements vector used to assist with debugging 
void DisjointSets::printElements() {
	if (elements.size() == 0) {
		cout << "vector<int> elements is empty" << endl;
		return;
	}
	printElement(0);
	for (size_t i = 1; i < elements.size(); i++) {
		cout << ", ";
		printElement(i);
	}
	cout << endl;
}

//Helper method to print out a single element from elements used to assist with debugging 
void DisjointSets::printElement(int index) {
	cout << "[" << index << "]: " << elements[index];
}