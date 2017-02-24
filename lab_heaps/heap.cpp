/**
 * @file heap.cpp
 * Implementation of a heap class.
 */

using namespace std;

template <class T, class Compare>
size_t heap<T, Compare>::root() const {
    return 0;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const {
    return currentIdx*2 + 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const {
    return currentIdx*2 + 2;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const {
    return (currentIdx-1)/2;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const {
    return _elems.size()-1 >= currentIdx*2+1;
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const {
    size_t left  = leftChild(currentIdx);
    size_t right = rightChild(currentIdx);

    if (_elems.size()-1 >= currentIdx*2+2) {
        return higherPriority(_elems[left], _elems[right]) ? left : right;
    } else {
        return left;
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx) {
    if (!hasAChild(currentIdx))
        return;
    heapifyDown(leftChild(currentIdx));
    heapifyDown(rightChild(currentIdx));
    size_t child = maxPriorityChild(currentIdx);
    if (higherPriority(_elems[child], _elems[currentIdx])) {
        std::swap(_elems[currentIdx], _elems[child]);
        heapifyDown(child);
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx) {
    if (currentIdx == root())
        return;
    size_t parentIdx = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
        std::swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(parentIdx);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap() { }

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems) {
    for (auto elem: elems) {
        _elems.push_back(elem);
    }
    if (!empty())
        heapifyDown(root());
}

template <class T, class Compare>
T heap<T, Compare>::pop() {
    if (empty())
        return T();
    size_t lastIdx = _elems.size()-1;
    std::swap(_elems[root()], _elems[lastIdx]);
    T retVal = _elems[lastIdx];
    _elems.pop_back();
    if (!empty())
        heapifyDown(root());
    return retVal;
}


template <class T, class Compare>
T heap<T, Compare>::peek() const {
    if (empty())
        return T();
    T retVal = _elems[root()];
    return retVal;
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem) {
    _elems.push_back(elem);
    heapifyUp(_elems.size()-1);
}

template <class T, class Compare>
bool heap<T, Compare>::empty() const {
    return _elems.size() == 0;
}
