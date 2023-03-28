/*******************************************************************
*   Author:          Matthew Lingenfelter                          *
*   Major:           Computer Science                              *
*   Creation Date:   February 8, 2023                              *
*   Course:          Data Structures                               *
*   Filename:        AvlTree.h                                     *
*   Purpose:         Takes a user given sequence of integers,      *
*           builds an AVL tree with them non-recursivly. Can       *
*           provide the k'th smallest node, the total number of    *
*           nodes, and can print the tree in sorted order,         *
*           pre-ordered, and post-order.                           *
*******************************************************************/
#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "dsexceptions.h"
#include <iostream>    // For NULL
using namespace std;

/*
* AvlTree class
*
* CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
*
************ PUBLIC OPERATIONS ****************
* void insert(x)         --> Inserts x
* bool contains(x)       --> Returns true if x is present
* Comparable findMin()   --> Returns smallest item
* Comparable findMax()   --> Returns largest item
* bool isEmpty()         --> Returns true if empty
* void makeEmpty()       --> Removes all items
* void printTree()       --> Prints tree in sorted order
* int findKth(k) --> Returns the k'th smallest element
* void preOrder() --> Prints the tree in sorted pre order
* void postOrder() --> Prints tree in sorted post order
* int count() --> Returns total number of nodes
********************************************
* Throws UnderflowException as warratned
*/

template <typename Comparable>
class AvlTree {
    public:
        AvlTree() : root(NULL)
            { }
        AvlTree(const AvlTree &rhs) : root(NULL) {
            *this = rhs;
        }

        ~AvlTree() {
            makeEmpty();
        }

        // Find the smallest item in the tree
        // Throws UnderflowException if empty
        const Comparable &findMin() const {
            if(isEmpty())a
                throw UnderflowException();
            return findMin(root)->element;
        }

        // Find the largest item in the tree
        // Throws UnderflowException if empty
        const Comparable &findMax() const {
            if(isEmpty())
                throw UnderflowException();
            return findMax(root)->element;
        }

        // Returns true if x is found in the tree
        bool contains(const Comparable &x) const {
            return contains(x, root);
        }

        // Tests if the tree is logically empty
        // Returns true if empty, false otherwise
        bool isEmpty() const {
            return root == NULL;
        }

        // Prints the tree contents in sorted order
        void printTree() const {
            if(isEmpty())
                cout << "Empty tree\n";
            else
                printTree(root);
        }

        // Make the tree logically empty
        void makeEmpty() {
            makeEmpty(root);
        }

        // Insert x into the tree, duplicants are ignored
        void insert(const Comparable &x) {
            insert(x, root);
        }

        // Deep copy
        const AvlTree &operator=(const AvlTree &rhs) {
            if(this != &rhs) {
                makeEmpty();
                root = clone(rhs.root);
            }
            return *this;
        }

        // Finds and returns the k'th smallest element in the tree
        int findKth(const Comparable &k) {
            return findKth(k, root);
        }

        // Prints the tree contents using pre-order traversal
        void preOrder() const {
            if(isEmpty())
                cout << "Empty tree\n";
            else
                preOrder(root);
        }

        // Prints the tree contents using post-order traversal
        void postOrder() const {
            if(isEmpty())
                cout << "Empty tree\n";
            else
                postOrder(root);
        }

        // Returns the total number of nodes in the tree
        int count() const {
            int c;
            if(isEmpty())
                c = 0;
            else
                c = count(0, root);
            return c;
        }

    private:
        struct AvlNode {
            Comparable element;
            AvlNode *left;
            AvlNode *right;
            int height;

            AvlNode(const Comparable &theElement, AvlNode *lt, AvlNode *rt, int h = 0)
                : element(theElement), left(lt), right(rt), height(h){}
        };

    AvlNode *root;

    // Internal method to insert into a subtree
    // x is the item to insert
    // t is the node that roots the subtree
    // Sets the new root of the subtree
    void insert(const Comparable &x, AvlNode *&t) {
        // The tree is empty, x becomes the root node
        if(t == NULL) {
            t = new AvlNode(x, NULL, NULL);
            t->height = max(height(t->left), height(t->right)) + 1;
        }
        else {
            int index, h, c;
            AvlNode *path[t->height + 1];
            AvlNode *node, *w, *y, *z;
            AvlNode **r;

            // Loops to find the position of x                               
            // Saves the path from the root to x                             
            for(index = 0, r = &t; (node = *r); ++index) {
                node->height = max(height(node->left), height(node->right)) + 2;
                path[index] = node;

                if(x < node->element)
                    r = &node->left;
                else if(x > node->element)
                    r = &node->right;
                else
                    return; //ignore duplicates
            }

            path[index] = *r = new AvlNode(x, NULL, NULL);

            // loops backwards, from x (up to) the root of the tree          
            // this checks for Avl violations and rebalances the tree        
            for(h = 1; index; ++h) {
                w = path[--index];

                // if the height of x is < 2 the tree at w is balanced         
                if(w->height >= 2) {
                    // since the height is >= 2, if either child is Null         
                    // or the difference in height between the children          
                    // is > 1, the tree at w is unbalanced                       
                    if((w->left == NULL) || (w->right == NULL) || (c = w->left->height - w->right->height) < -1 || c > 1) {
                        y = path[index + 1];
                        z = path[index + 2];

                        // checks which type of violation has occured and          
                        // balances the tree accordingly                           
                        if(y ==  w->left) {
                            if(z == y->left)
                                rotateWithLeftChild(w);
                            else
                                doubleWithLeftChild(w);
                        }
                        else {
                            if(z == y->left)
                                doubleWithRightChild(w);
                            else
                                rotateWithRightChild(w);
                        }

                        // after rotating, fix parent link to newly                
                        // ordered subtree                                         
                        if(index > 0) {
                            if(path[index]->element > path[index-1]->element)
                                path[index-1]->right = w;
                            else
                                path[index-1]->left = w;
                        }
                        else
                        t = w; // becomes the root of the total tree             
                        break;
                    }
                }
            }
        }
    }

    // Internal method to find the smallest item in a subtree t
    // Returns the node containing the smallest item
    AvlNode * findMin( AvlNode *t ) const {
        if( t == NULL )
            return NULL;
        if( t->left == NULL )
            return t;
        return findMin( t->left );
    }

    // Internal method to find the larget item in a subtree t
    // Returns the node containing the larget item
    AvlNode * findMax( AvlNode *t ) const {
        if( t != NULL )
            while( t->right != NULL )
                t = t->right;
        return t;
    }

    // Internal method to test if an item is in a subtree
    // x is the item to search for
    // t is the node that roots the tree
    bool contains( const Comparable & x, AvlNode *t ) const {
        if( t == NULL )
            return false;
        else if( x < t->element )
            return contains( x, t->left );
        else if( t->element < x )
            return contains( x, t->right );
        else
            return true;    // Match                                         
    }

    // Internal method to make subtree empty
    void makeEmpty( AvlNode * & t ) {
        if( t != NULL ) {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = NULL;
    }

    // Internal method to print a subtree rooted at t in sorted order
    void printTree( AvlNode *t ) const {
        if( t != NULL ) {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }
    }
    
    // Internal methos to clone subtree
    AvlNode * clone( AvlNode *t ) const {
        if( t == NULL )
            return NULL;
        else
            return new AvlNode( t->element, clone( t->left ), clone( t->right ), t->height );
    }

    // Avl manipulations

    //Returns the height of node t or -1 if NUll
    int height( AvlNode *t ) const {
        return t == NULL ? -1 : t->height;
    }

    int max( int lhs, int rhs ) const {
        return lhs > rhs ? lhs : rhs;
    }

    // Rotate binary tree node with left child
    // For AvlTrees, this is a single rotation for case 1
    // Updates heights, then sets new root
    void rotateWithLeftChild( AvlNode * & k2 ) {
        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;
    }

    // Rotate binary tree node with right child
    // For AvlTrees, this is a single rotation for case 4
    // Updates heights, then sets new root
    void rotateWithRightChild( AvlNode * & k1 ) {
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k2->height = max( height( k2->right ), k1->height ) + 1;
        k1 = k2;
    }

    // Double rotate binary tree node: first left child
    // with its right child; then node k3 with new left child
    // For AvlTrees, this is a double rotation for case 2
    // Updates heights, then sets new root
    void doubleWithLeftChild( AvlNode * & k3 ) {
        // rotate right with left child                                    
        AvlNode *k1 = k3->left;
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;

        // rotate with new left child                                      
        k3->left = k2->right;
        k2->right = k3;
        k3->height = max(height(k3->left), height(k3->right)) + 1;
        k2->height = max(height(k2->left), k3->height) + 1;
        k3 = k2;
    }

    // Double rotate binary tree node: first right child
    // with its left child; then node k1 with new right child
    // For AvlTrees, this is a double rotation for case 3
    // Updates height, then sets new root
    void doubleWithRightChild( AvlNode * & k1 ) {
        // rotate left with right child                                    
        AvlNode *k3 = k1->right;
        AvlNode *k2 = k3->left;
        k3->left = k2->right;
        k2->right = k3;
        k3->height = max(height(k3->left), height(k3->right)) + 1;
        k2->height = max(height(k2->left), k3->height) + 1;

        // rotate with new right child                                     
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }

    /*****************************************************************   
    *   Function Name: findKth                                       *   
    *   Description:   Returns the k'th smallest node in the AVL tree*   
    *   Parameters:    const Comparable &k: the value of k           *   
    *                  AvlNode *&t: root of the subtree              *   
    *   Return Value:  int - the k'th smallest integer in the tree   *   
    *****************************************************************/
    int kth = 0;
    int findKth(const Comparable & k, AvlNode * & t) {
        if(t == NULL)
            return -1;

        // Searching left subtree                                          
        int l = findKth(k, t->left);

        // if the k'th smallest node is in the left subtree, return it     
        if(l > 0)
            return l;

        // if the current node is the k'th smallest, return it             
        kth++;
        if(kth == k)
            return t->element;

        // else, search the right subtree                                  
        return findKth(k, t->right);
    }

    /*****************************************************************   
    *   Function Name: preOrder                                      *   
    *   Description:   Prints the AvlTree using pre-order traversal  *   
    *   Parameters:    AvlNode *t: root of the subtree               *   
    *   Return Value:  none                                          *   
    *****************************************************************/
    void preOrder( AvlNode *t ) const {
        if(t == NULL)
            return;
        cout << t->element << " ";
        preOrder(t->left);
        preOrder(t->right);
    }

    /*****************************************************************   
    *   Function Name: preOrder                                      *   
    *   Description:   Prints the AvlTree using post-order traversal *   
    *   Parameters:    AvlNode *t: root of the subtree               *   
    *   Return Value:  none                                          *   
    *****************************************************************/
    void postOrder( AvlNode *t ) const {
        if(t == NULL)
            return;
        postOrder(t->left);
        postOrder(t->right);
        cout << t->element << " ";
    }

    /*****************************************************************   
    *   Function Name: count                                         *   
    *   Description:   Counts the total number of nodes in           *   
    *                    the AvlTree                                 *   
    *   Parameters:    int c: current count total                    *   
    *                  AvlNode *t: root of the subtree               *   
    *   Return Value:  int - the total number of nodes               *   
    *****************************************************************/
    int count(int c, AvlNode *t ) const {
        if( t != NULL ) {
            c = count(c, t->left );
            c = count(c, t->right );
            return c + 1;
        }
        return c;
    }
};

#endif