/**
 * @file binarytree.h
 * Declaraction of the BinaryTree class. You will probably need to modify this
 *  file to add helper functions.
 */

#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <sstream>
#include "random.h"
#include <iomanip>
#include <queue>

using namespace std;

/**
 * The BinaryTree class
 */
class BinaryTree
{

public:
    /**
     * Represents a tree node; that is, an element in a BinaryTree.
     * It has a data element and pointers to its left and right children.
     */
    struct Node {
        int elem;
        Node* left;
        Node* right;
        Node* neighbor;

        /**
         * Node element constructor; sets children to point to NULL.
         * @param element The templated data element that the constructed
         *  node will hold.
         */
        Node(const int& element) : elem(element), left(NULL), right(NULL)
        { /* nothing */

        }
    };

    /* Constructor to create an empty tree. */
    BinaryTree();

    /**
     * Destructor; frees all nodes associated by this tree.
     */
    ~BinaryTree();

    /**
     * Frees all nodes associated with this tree and sets it to be empty.
     */
    void clear();

    /**
     * Inserts into the BinaryTree.
     * @param elem The element to insert
     * @param sorted By default, this parameter is false. That means that the
     *  element takes a pseudo-random path to a leaf where it is inserted. If
     *  true, the insert function will act like it does in a BST.
     */
    void insert(const int& elem, bool sorted);

    /**
     * Prints the contents of the tree to stdout.
     */
    void print() const;

    /**
     * Returns a reference to the root
     */
    Node* & getRoot() ;

    /* The function(s) you must implement for question 9. See your problem
     * description for details. */
    bool isFullBinaryTree() const;
    bool isFullBinaryTree(Node* node) const;
    
    /* The function(s) you must implement for question 10. See your problem
     * description for details. */
    void makeNeighbors();

    Node* root;

    /**
     * Helper function for the public insert function.
     * @param node The current node in the recursion
     * @param elem The element to insert
     * @param sorted By default, this parameter is false. That means that the
     *  element takes a pseudo-random path to a leaf where it is inserted. If
     *  true, the insert function will act like it does in a BST.
     */
    void insert(Node*& node, const int& elem, bool sorted);

    /**
     * Helper function for clear that clears beneath the parameter node.
     * @param subRoot The current node in the recursion
     */
    void clear(Node* subRoot);

    /**
     * Put your own helper functions here.
     */
};

#endif
