/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */
#include <math.h>
#include <stdlib.h>
using std::swap;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const {
    return first[curDim] < second[curDim] || 
          (first[curDim] == second[curDim] && first < second);
}

template <int Dim>
bool KDTree<Dim>::smallerRadius(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential, int curDim) const {
    double currentBestRadius = 0;
    for(int i = 0; i < Dim; i++) {
        currentBestRadius += pow(target[i] - currentBest[i], 2);
    }
    double radius = pow(potential[curDim] - target[curDim], 2);
    return radius < currentBestRadius || 
          (radius == currentBestRadius && potential < currentBest);
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const {
    double distCurrBest  = 0;
    double distPotential = 0;
    for (int i = 0; i < Dim; i++) {
        distCurrBest  += pow(currentBest[i] - target[i], 2);
        distPotential += pow(potential[i]   - target[i], 2);
    }
    return distPotential < distCurrBest || 
          (distPotential == distCurrBest && potential < currentBest);
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints) {
    points = newPoints;
    sortPoints(0, points.size() - 1, 0);
}

template <int Dim>
void KDTree<Dim>::sortPoints(int startIndex, int endIndex, int dimension) {
    if (startIndex >= endIndex)
        return;
    int k = (startIndex + endIndex)/2;
    selectPivot(startIndex, endIndex, k, dimension);
    sortPoints(startIndex, k-1, (dimension+1)%Dim);
    sortPoints(k+1, endIndex,   (dimension+1)%Dim);
}

template <int Dim>
void KDTree<Dim>::selectPivot(int startIndex, int endIndex, int k, int dimension) {
    while(true) {
        int pivotIndex = pivot(startIndex, endIndex, dimension);
        if (k == pivotIndex) {
            break;
        } else if (k < pivotIndex) {
            endIndex   = pivotIndex-1;
        } else {
            startIndex = pivotIndex+1;
        }
    }
}

template <int Dim>
int KDTree<Dim>::pivot(int p, int r, int dimension) {
    int i = p-1;
    Point<Dim> x = points[r];
    for(int j = p; j < r; j++) {
        if (smallerDimVal(points[j], x, dimension)) {
            i++;
            swap(points[i], points[j]);
        }
    }
    swap(points[i+1], points[r]);
    return i+1;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& target) const {
    if (points.size() == 0) {
        return Point<Dim>();
    }
    int medianIndex = (points.size() - 1)/2;
    return findNearestNeighbor(target, points[medianIndex], 0, points.size() - 1, 0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& target, Point<Dim> currentBest, int startIndex, int endIndex, int dimension) const {
    if (startIndex > endIndex)
        return currentBest;

    if (startIndex == endIndex) {
        if (shouldReplace(target, currentBest, points[startIndex]))
            currentBest = points[startIndex];
        return currentBest;
    } else {
        int medianIndex = (startIndex + endIndex)/2;
        int targetTreeL = medianIndex + 1, targetTreeT = endIndex;
        int opposiTreeL = startIndex,      opposiTreeT = medianIndex - 1;

        if (smallerDimVal(target, points[medianIndex], dimension)) {
            swap(targetTreeL, opposiTreeL);
            swap(targetTreeT, opposiTreeT);
        }

        currentBest = findNearestNeighbor(target, currentBest, targetTreeL, targetTreeT, (dimension+1)%Dim);
        if (shouldReplace(target, currentBest, points[medianIndex])) 
            currentBest = points[medianIndex];
        return smallerRadius(target, currentBest, points[medianIndex], dimension) ? 
               findNearestNeighbor(target, currentBest, opposiTreeL, opposiTreeT, (dimension+1)%Dim) : 
               currentBest;;
    }
    return Point<Dim>();
}






