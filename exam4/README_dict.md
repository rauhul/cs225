You will be implementing the following function:

```
pair<int,int> getPair(int arr[], int n, int sum);
```

Given an unsorted array of integers that may contain duplicates, its length
`n`, and an integer `sum`, write a function that returns a `std::pair` of array
elements whose squares add up to the given sum. If there is no such pair of
elements, return the pair `(0,0)`.

You can assume that the array contains no `0` values. 

Examples:

- `arr = {1, 2, 3}`, `sum = 4` should return `(0,0)` (No pairs exist)
- `arr = {2, 3, 2, 1}`, `sum = 8` should return `(2,2)` (`2*2 + 2*2 == 4 + 4 == 8`) 
- `arr = {1, 1, 1, 1}`, `sum = 2` should return `(1,1)`

You may find the <a href="<%= clientFile('cpp-reference/en/cpp/utility/pair/make_pair.html') %>" target="_blank">make_pair</a> function useful.
