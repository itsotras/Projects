#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    /*BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');
   */
    // AVL Tree Tests
    AVLTree<int,std::string> at;
    at.insert(std::make_pair(5,"five"));
    //at.insert(std::make_pair(7,"seven"));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<int,std::string>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    cout << "Erasing 7" << endl;
    at.remove(7);
    cout << "Erasing 5" << endl;
    at.remove(5);

    return 0;
}
