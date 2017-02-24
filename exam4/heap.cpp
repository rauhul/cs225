#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

vector<int> findRightArm(MinHeap minheap) {
    vector<int> rightArm = vector<int>();
    size_t index = 1;
    while (minheap.elements.size() > index) {
//        cout << minheap.elements[index] << endl;
        rightArm.push_back(minheap.elements[index]);
        index = index * 2 + 1;
    }
    return rightArm;
}
