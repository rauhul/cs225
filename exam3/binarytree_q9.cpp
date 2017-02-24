#include "binarytree.h"

bool BinaryTree::isFullBinaryTree() const {
    return isFullBinaryTree(root);
}

bool BinaryTree::isFullBinaryTree(Node* node) const {
    if (node == NULL) {
        return true;
    }
    
    if ((node->left == NULL && node->right != NULL) || (node->left != NULL && node->right == NULL)) {
        return false;    
    }   
    
    return isFullBinaryTree(node->left) && isFullBinaryTree(node->right);
}
