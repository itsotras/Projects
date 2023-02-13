#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    bool isLeftChild(AVLNode<Key,Value>* p,  AVLNode<Key,Value>* n);
    bool isRightChild(AVLNode<Key,Value>* p,  AVLNode<Key,Value>* n);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);
    void insert_fix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
    void remove_fix(AVLNode<Key,Value>* node, char diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(AVLTree<Key,Value>::root_ == nullptr){//if empty tree
        AVLNode<Key, Value>* node =  new typename AVLNode<Key,Value>::AVLNode(new_item.first, new_item.second, nullptr); //create new node
        node->setBalance(0);//set balance to 0
        this->root_ = node;//set root to me
    }
    else{
        AVLNode<Key, Value>* walk = static_cast<AVLNode<Key,Value>*>(this->root_); //walk down the tree
        AVLNode<Key, Value>* newParent = nullptr; //get my new Parent
        while(walk != nullptr){
            if(new_item.first < walk->getKey()){ //if I am less than this node traverse to the left
                newParent = walk;
                walk = walk->getLeft();
            }
            else if(new_item.first > walk->getKey()){ //If I am greater than this node traverse to the right
                newParent = walk;
                walk = walk->getRight();
            }
            else if(new_item.first == walk->getKey()){ //otherwise, update this value
                walk->setValue(new_item.second);
                return;
            }
        }
        AVLNode<Key, Value>* node = new typename AVLNode<Key,Value>::AVLNode(new_item.first, new_item.second, newParent); //initialize new node with balance of 0
        node->setBalance(0);
        if(new_item.first < newParent->getKey()){ //put node to the left
            newParent->setLeft(node);
        }
        else if(new_item.first > newParent->getKey()){ //put node to the right
            newParent->setRight(node); 
        }

        if(newParent->getBalance() == 1 || newParent->getBalance() == -1){//if its balance was 1 or -1
            newParent->setBalance(0); //set balance to 0
        }
        else{//if the balance was already 0
            if(isLeftChild(newParent, node)){//if I am the left child
                newParent->updateBalance(-1); //update balance of parent to -1
            }
            else{ //if I am the right child
                newParent->updateBalance(1); //update balance of parent to 1
            }
            
            insert_fix(newParent, node); //fix this tree
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    Node<Key, Value>* node = static_cast<Node<Key,Value>*>(this->root_);//get the root node
    //this->printRoot(this->root_);
    
    while(node != nullptr){// traverse the tree
            if(key < node->getKey()){ //if this key is less than the current node key
                node = node->getLeft();
            }
            else if(key > node->getKey()){ //if this key is greater than the current node key
                node = node->getRight();
            }
            else{//if this key is equal to the current node key
                break;
            }
        }
    
    if(node == nullptr){//if not found
        return;
    }
    else if(node == this->root_){ //if the root exists and I am the root
        if(node->getLeft() == nullptr && node->getRight()== nullptr){//if I have no children
            this->root_ = nullptr;//set root to null
            delete node;
            return;
        }
        else if(node->getLeft() != nullptr && node->getRight() == nullptr){//if I have one left child
            this->root_ = node->getLeft(); //set root to my left, promote my left child
            node->getLeft()->setParent(nullptr); //set my left's parent to null
            node->setLeft(nullptr); //set my left to null
            delete node;
            return;

        }
        else if(node->getRight() != nullptr && node->getLeft() == nullptr){//if I have one right child
            this->root_ = node->getRight(); //set root to my right, promote my right child
            node->getRight()->setParent(nullptr); //set right's parent to null
            node->setRight(nullptr); //set my right to null
            delete node;
            return;

        }
    }


    if(node->getLeft() != nullptr && node->getRight()!= nullptr){//if I have two children
        Node<Key, Value>* pred = BinarySearchTree<Key, Value>::predecessor(node); //get predecessor

        nodeSwap(static_cast<AVLNode<Key, Value>*>(node), static_cast<AVLNode<Key, Value>*>(pred) );//swap with predecessor
        if(this->root_ == node){//if I'm the root_ node
            this->root_ = pred; //set my predecessor to be the new root
        }
    }


    Node<Key, Value>* parent = node->getParent();//get the parent
    char diff = 0;
    if(parent != nullptr){//if i'm not the root
        if(isLeftChild(static_cast<AVLNode<Key, Value>*>(parent), static_cast<AVLNode<Key, Value>*>(node))){ //if I am the left child
            diff = 1; //removing me makes right bigger
        }
        else if(isRightChild(static_cast<AVLNode<Key, Value>*>(parent), static_cast<AVLNode<Key, Value>*>(node))){ //if I am the right child
            diff = -1; //removing me makes left bigger
        }
        if(node->getLeft() == nullptr && node->getRight() == nullptr){ //if I have no children and am not a root
            if(isLeftChild(static_cast<AVLNode<Key, Value>*>(parent), static_cast<AVLNode<Key, Value>*>(node))){//if I'm the left child of parent
                parent->setLeft(nullptr); //set parent's left to null
                node->setParent(nullptr); //set my parent to null
            }
            else{
                parent->setRight(nullptr); //if I'm the right child of parent
                node->setParent(nullptr); //set my parent to null
            }
        }
        else if(node->getLeft()!= nullptr){ //if i have a left child
            node->getLeft()->setParent(parent); //set my left parent to my parent
            
            if(isLeftChild(static_cast<AVLNode<Key, Value>*>(parent), static_cast<AVLNode<Key, Value>*>(node))){ //if i am the left child of my parent
                parent->setLeft(node->getLeft()); //set parent's left to my left
            }
            else if(isRightChild(static_cast<AVLNode<Key, Value>*>(parent), static_cast<AVLNode<Key, Value>*>(node))){ //if i am the right child of my parent
                parent->setRight(node->getLeft()); //set parent's right to my left
            }
            node->setLeft(nullptr); //erase me and my connections
            node->setParent(nullptr);
        }
        else if(node->getRight()!= nullptr){//if I have a right child
            node->getRight()->setParent(parent);
            
            if(isLeftChild(static_cast<AVLNode<Key, Value>*>(parent), static_cast<AVLNode<Key, Value>*>(node))){ //if i am the left child of my parent
                parent->setLeft(node->getRight()); //set parent's left to my right
            }
            else if(isRightChild(static_cast<AVLNode<Key, Value>*>(parent), static_cast<AVLNode<Key, Value>*>(node))){ //if i am the right child of my parent
                parent->setRight(node->getRight()); //set parent's right to my right
            }
            node->setRight(nullptr); //erase me and my connections
            node->setParent(nullptr);

        }
    }
    
    delete node; //delete me 
    remove_fix(static_cast<AVLNode<Key, Value>*>(parent), diff);//send to get fixed
    
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key,Value>* p,  AVLNode<Key,Value>* n){
    if(p->getLeft() == n){
        return true;
    }
    return false;

}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key,Value>* p,  AVLNode<Key,Value>* n){
    if(p->getRight() == n){
        return true;
    }
    return false;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node){
    
    AVLNode<Key,Value>* leftChild = node->getLeft();
    AVLNode<Key,Value>* newChild = leftChild->getRight();
    if(node == this->root_){ //if I am the root node
        leftChild->setParent(nullptr);
        this->root_ = leftChild;//set my child to the new root
    }
    else{
        AVLNode<Key,Value>* myParent = node->getParent();
        if(isLeftChild(myParent, node)){//if I am the left child of my parent
            myParent->setLeft(leftChild);//set their left to my left
        }
        else{
            myParent->setRight(leftChild);//set their right to my left
        }
        leftChild->setParent(myParent);//set the parent of my child to its new parent

    }
    leftChild->setRight(node);//set me as my childs right child
    node->setParent(leftChild);//set my child as parent
    node->setLeft(newChild);//set my new child as the right child of my old left child
    if(newChild != nullptr){
        newChild->setParent(node); //set the new parent of my left child as me
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){

    AVLNode<Key,Value>* rightChild = node->getRight();
    AVLNode<Key,Value>* newChild = rightChild->getLeft();
    if(node == this->root_){ //if I am the root node
        rightChild->setParent(nullptr);
        this->root_ = rightChild;//set my child to the new root
    }
    else{
        AVLNode<Key,Value>* myParent = node->getParent();
        if(isLeftChild(myParent, node)){//if I am the left child of my parent
            myParent->setLeft(rightChild);//set their left to my right
        }
        else{
            myParent->setRight(rightChild);//set their right to my right
        }
        rightChild->setParent(myParent);//set the parent of my child to its new parent

    }
    rightChild->setLeft(node);//set me as my childs left child
    node->setParent(rightChild);//set my child as parent
    node->setRight(newChild);//set my new child as the right child of my old left child
    if(newChild != nullptr){
        newChild->setParent(node); //set the new parent of my left child as me
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node){
    if(parent == nullptr || parent->getParent() == nullptr){//base case
        return;
    }
    AVLNode<Key,Value>* gParent = parent->getParent();//get the grandparent
    if(isLeftChild(gParent, parent)){// if parent is left child of gParent
        gParent->updateBalance(-1);//add -1 because we have added new left child
        if(gParent->getBalance() == 0){ //if the new balance for g is 0
            return;
        }
        else if(gParent->getBalance() == -1){ //if it is -1, continue
            insert_fix(gParent, parent);
        }
        else if(gParent->getBalance() == -2){//if it is -2, stop and fix it before it spreads
            if(isLeftChild(parent, node)){ //if zig-zig
                rotateRight(gParent); //rotate right
                parent->setBalance(0);
                gParent->setBalance(0);

            }
            else{//if zig-zag
                rotateLeft(parent);
                rotateRight(gParent);
                if(node->getBalance() == -1){ //if it is -1
                    parent->setBalance(0);
                    gParent->setBalance(1);
                    node->setBalance(0);
                }
                else if(node->getBalance() == 0){ //if it is 0
                    parent->setBalance(0);
                    gParent->setBalance(0);
                    node->setBalance(0);

                }
                else if(node->getBalance() == 1){ //if it is 1
                    parent->setBalance(-1);
                    gParent->setBalance(0);
                    node->setBalance(0);
                }
            }

        }
    }
    else{
        gParent->updateBalance(1);//add 1 because we have added new right child
        if(gParent->getBalance() == 0){
            return;
        }
        else if(gParent->getBalance() == 1){ //if it has a balance weight of 1, continue recursing
            insert_fix(gParent, parent);
        }
        else if(gParent->getBalance() == 2){ //if it has a weight of 2, fix it
            if(isRightChild(parent, node)){ //if zig-zig
                rotateLeft(gParent); //rotate left
                parent->setBalance(0);
                gParent->setBalance(0);

            }
            else{//if zig-zag
                rotateRight(parent);
                rotateLeft(gParent); 
                if(node->getBalance() == 1){//if weight is 1
                    parent->setBalance(0);
                    gParent->setBalance(-1);
                    node->setBalance(0);
                }
                else if(node->getBalance() == 0){ //if weight is 0
                    parent->setBalance(0);
                    gParent->setBalance(0);
                    node->setBalance(0);

                }
                else if(node->getBalance() == -1){//if weight is -1
                    parent->setBalance(1);
                    gParent->setBalance(0);
                    node->setBalance(0);
                }
            }

        }
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key,Value>* node, char diff){
    if(node == nullptr){ //if this is null
        return;
    }
    AVLNode<Key, Value>* parent = node->getParent();//get my parent
    char ndiff = 0;
    if(parent != nullptr){//if parent exists
        if(isLeftChild(parent, node)){
            ndiff = 1;
        }
        else{
            ndiff = -1;
        }
    }

    if(diff == -1){//if diff is -1
        if(node->getBalance() + diff == -2){
            AVLNode<Key, Value>* tallerChild = node->getLeft();
            if(tallerChild->getBalance() == -1){
                rotateRight(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                remove_fix(parent, ndiff);
            }
            else if(tallerChild->getBalance() == 0){
                rotateRight(node);
                node->setBalance(-1);
                tallerChild->setBalance(1);

            }
            else if(tallerChild->getBalance() == 1){
                AVLNode<Key, Value>* grandChild = tallerChild->getRight();
                rotateLeft(tallerChild);
                rotateRight(node);
                if(grandChild->getBalance() == 1){
                    node->setBalance(0);
                    tallerChild->setBalance(-1);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == 0){
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == -1){
                    node->setBalance(1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                remove_fix(parent, ndiff);

            }

        }
        else if(node->getBalance() + diff == -1){
            node->setBalance(-1);
        }
        else if(node->getBalance() + diff == 0){
            node->setBalance(0);
            remove_fix(parent, ndiff);


        }
    }
    else if(diff == 1){//if diff is 1
        if(node->getBalance() + diff == 2){
            AVLNode<Key, Value>* tallerChild = node->getRight();
            if(tallerChild->getBalance() == 1){
                rotateLeft(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                remove_fix(parent, ndiff);
            }
            else if(tallerChild->getBalance() == 0){
                rotateLeft(node);
                node->setBalance(1);
                tallerChild->setBalance(-1);

            }
            else if(tallerChild->getBalance() == -1){
                AVLNode<Key, Value>* grandChild = tallerChild->getLeft();
                rotateRight(tallerChild);
                rotateLeft(node);
                if(grandChild->getBalance() == -1){
                    node->setBalance(0);
                    tallerChild->setBalance(1);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == 0){
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                else if(grandChild->getBalance() == 1){
                    node->setBalance(-1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                remove_fix(parent, ndiff);

            }

        }
        else if(node->getBalance() + diff == 1){
            node->setBalance(1);
        }
        else if(node->getBalance() + diff == 0){
            node->setBalance(0);
            remove_fix(parent, ndiff);


        }
    }

}


#endif
