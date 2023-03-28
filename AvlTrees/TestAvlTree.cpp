/*******************************************************************
*   Author:         Matthew Lingenfelter                           *
*   Major:          Computer Science                               *
*   Creation Date:  February 8, 2023                               *
*   Course:         CSC402                                         *
*   Purpose:        Propmts user for a sequence of integers,       *
*           builds an AVL tree with them, prints the total number  *
*           of nodes, prints the pre-order and post-order          *
*           traversal sequences of the tree. Then tests the        *
*           contains() method and the findkth() method.            *
*******************************************************************/
#include <iostream>
#include "AvlTree.h"
using namespace std;

// Test program                                                    
int main( ){
    AvlTree<int> t;
    int i, c, v, k, kth;

    // prompts user for a sequence of integers                         
    cout<<"Enter a sequence of integers to build the AVL tree ";
    cout<<"(enter -1 to end): ";
    cin >> i;

    // reads in integers until a -1 is read,                           
    // and builds the Avl Tree                                         
    while(i != -1) {
      t.insert(i);
      cin >> i;
    }

    c = t.count();
    cout << "\nThe Avl tree has " << c << " nodes in total.\n";

    cout << "\nPre-order tree traversal: ";
    t.preOrder();

    cout << "\n\nPost-order tree traversal: ";
    t.postOrder();

    // gets the value to find in the tree from user                    
    cout << "\n\nSearch for a value: ";
    cin >> v;

    // if v is in tree or not                                          
    if(t.contains(v))
      cout << "\n" << v << " is in the AVL tree.\n";
    else
      cout << "\n" << v << " is not in the AVL tree.\n";

    // gets the value of k from user                                   
    cout << "\nSearch fot the k'th smallest value. Enter k: ";
    cin >> k;

    // prints the k'th smallest element in the tree                    
    // prints a -1 if not found                                        
    kth = t.findKth(k);
    cout << "\nThe " <<k;
    switch(k) {
      case 1:
        cout << "'st ";
        break;
      case 2:
        cout << "'nd ";
        break;
      case 3:
        cout << "'rd ";
        break;
      default:
        cout << "'th ";
        break;
    }
    cout << "smallest value in the AVL tree is " << kth << ".\n\n";

    return 0;
}