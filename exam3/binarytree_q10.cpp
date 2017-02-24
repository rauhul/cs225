#include "binarytree.h"
#include <queue>
using std::queue;
void BinaryTree::makeNeighbors() {

    /*
    queue<Node*> q;
    root->height = 0;
    q.push(root);
    
    while (!q.empty()) {
        Node* node = q.front();
        q.pop();
        if (node != NULL) {
            if (node->left != NULL) {
                node->left->height = node->height + 1;
                q.push(node->left);
            }
            if (node->right != NULL) {
                node->right->height = node->height + 1;
                q.push(node->right);
            }
            if (q.front() != NULL) {
                if (node->height == q.front()->height) {
                    node->neighbor = q.front();
                } else {
                    node->neighbor = NULL;
                }
            } else {
                node->neighbor = NULL;
            }
        }
    }
    
    return;
    
    */
    
    
    queue<Node*> currHeight;
    queue<Node*> nextHeight;
    
    currHeight.push(root);
    while(!currHeight.empty()) {
        Node* node = currHeight.front();
        currHeight.pop();
        if (node != NULL) {
            if (currHeight.empty()) {
                node->neighbor = NULL;
            } else {
                node->neighbor = currHeight.front();
            }
            nextHeight.push(node->left);
            nextHeight.push(node->right);
        }
        
        if (currHeight.empty()) {
            currHeight = nextHeight;
            nextHeight = queue<Node*>();
        }
    }
    
    /*
    
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* node = q.front();
        q.pop();
        
        
        if (node != NULL) {
        cout << node << "," << node->neighbor << endl;
        q.push(node->left);
        q.push(node->right);

        }
    
    }
    */
    
    return;
    
}
