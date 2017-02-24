You will be implementing the following function:

```
vector<int> findRightArm(MinHeap minheap)
```

Given a `MinHeap` (see `MinHeap.h`), return the right arm of the heap
(including the root) as a `std::vector<int>`. The root of heap corresponds to
index 1 in the `elements` vector.

For example, given a `MinHeap` of `[-inf, 5, 8, 12, 9, 17, 15]`, you should
return `[5, 12]` (returned vector should start at index 0). If the heap is
empty, return an empty `std::vector<int>`.

Visual aid (right arm is marked with `*`): 
```
      5*
    /   \
   8    12*
  / \   / 
 9  17 15
```
