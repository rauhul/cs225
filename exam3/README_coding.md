#### Question 9

You will be implementing the following methods in the `BinaryTree` class:

- `bool isFullBinaryTree() const;`

Given a binary tree, determine if it is a full binary tree.

**A full binary tree is a tree in which every node other than the leaves has two children.**

Write a function `isFullBinaryTree()` that determines whether the given binary tree is a full binary tree or not.

Your function should return a boolean value. 

The following trees are full binary trees.

```
    _9_                 _9_
   /   \               /   \
  3     2             4     5 
 / \   / \                 / \
4   1 7   6               7   8
```

The following tree is not a full binary tree.

```
      9
     / \
    3   2
   /   / 
  4   6
 /
5
```

**Write your solution in `binarytree_q9.cpp`.**


- - -

#### Question 10

You will be implementing the following methods in the `BinaryTree` class:

- `void makeNeighbors();`

Write a function `makeNeighbors()` that connects the nodes of a binary tree to
the immediate right node of the tree. 

The connections are made level wise and the rightmost node in the level
connects to `NULL`. 

Make a member variable in the node struct called `neighbor` (along with left
and right) of type `Node *` in the following manner:

`Node * neighbor`;

Example, the following tree: 

```
     9
    / \
   3   2
  / \   \
 4   1   6
```

must become:

```
     9 —> NULL
    / \
   3 —>2 —> NULL
  / \    \
 4 ->1 —> 6 —> NULL
```

where the horizontal links represent the neighbor links. 

*Hint: You can use a `std::queue` to do this.*

Here are the function signatures you might need: (`T` is the queue's template
parameter.)

- `bool empty() const;`
- `T& front();`
- `void push (const T& val);`
- `void pop();`
- `T& back();`

**Write your solution in `binarytree_q10.cpp`.**
