#pragma once

#ifndef BST_H
#define BST_H

#include <iostream>
#include <functional>
#include <cassert>

template<typename K, typename I>
class BST
{
public:
    using KeyType = K;
    using ItemType = I;

    BST() = default; // Constructor
    BST(const BST<KeyType, ItemType>&); // Copy constructor
    BST<KeyType, ItemType>& operator=(const BST<KeyType, ItemType>&); // Copy assignment constructor
    BST(BST<KeyType, ItemType>&&); // Move constructor
    BST<KeyType, ItemType>& operator=(BST<KeyType, ItemType>&&); //Move assignment constructor
    ~BST(); // Destructor

    ItemType* lookup(KeyType);
    void insert(KeyType, ItemType);
    void displayEntries();
    void remove(KeyType);
    void removeIf(std::function<bool(KeyType)>);
    void displayTree();
    void rotateTest();
    void rotateLTest();

private:
    struct Node;
    Node* root = leaf();

    static Node* leaf();
    static bool isLeaf(Node*);
    ItemType* lookupRec(KeyType, Node*);
    void displayTreeRec(Node*&, int);
    void insertRec(KeyType, ItemType, Node*&);
    void displayEntriesRec(Node*&);
    void removeRec(Node*&, KeyType);
    void removeIfRec(std::function<bool(KeyType)>, Node*&);
    Node* detachMinimumNode(Node*&);
    void deepDelete(Node*); // recursive worker performing deep delete
    Node* deepCopy(Node*);
    void rotateLeft(Node*&);
    void rotateRight(Node*&);
};


template<typename K, typename I>
struct BST<K, I>::Node
{
    Node(KeyType, ItemType);
    KeyType key;
    ItemType item;
    Node* leftChild;
    Node* rightChild;
};

template<typename K, typename I>
typename BST<K, I>::Node* BST<K, I>::leaf()
{
    return nullptr;
}

template<typename K, typename I>
bool BST<K, I>::isLeaf(Node* n)
{
    return (n == leaf());
}

template<typename K, typename I>
BST<K, I>::Node::Node(KeyType key, ItemType item)
{
    this->key = key;
    this->item = item;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

template<typename K, typename I>
typename BST<K, I>::ItemType* BST<K, I>::lookup(KeyType soughtKey)
{
    return lookupRec(soughtKey, root);
}

template<typename K, typename I>
typename BST<K, I>::ItemType* BST<K, I>::lookupRec(KeyType soughtKey, Node* currentNode)
{
    if (isLeaf(currentNode)) return nullptr;
    else if (soughtKey == currentNode->key) return &(currentNode->item);
    else if (currentNode->key > soughtKey) return lookupRec(soughtKey, currentNode->leftChild);
    else if (currentNode->key < soughtKey) return lookupRec(soughtKey, currentNode->rightChild);
}

template<typename K, typename I>
void BST<K, I>::insert(KeyType k, ItemType i)
{
    insertRec(k, i, root);
}

template<typename K, typename I>
void BST<K, I>::insertRec(KeyType k, ItemType i, Node*& currentNode)
{
    if (isLeaf(currentNode)) currentNode = new Node(k, i);
    else if (currentNode->key == k) currentNode->item = i;
    else if (currentNode->key > k) insertRec(k, i, currentNode->leftChild);
    else if (currentNode->key < k) insertRec(k, i, currentNode->rightChild);
}

template<typename K, typename I>
void BST<K, I>::displayEntries()
{
    displayEntriesRec(root);
}

template<typename K, typename I>
void BST<K, I>::displayTree()
{
    displayTreeRec(root, 0);
}

template<typename K, typename I>
void BST<K, I>::displayTreeRec(Node*& currentNode, int depth)
{
    std::string tabs;
    for (size_t i = 0; i < depth; ++i) tabs += "\t";
    std::cout << tabs << currentNode->key << std::endl;

    if (!isLeaf(currentNode->leftChild)) displayTreeRec(currentNode->leftChild, depth + 1);
    if (!isLeaf(currentNode->rightChild)) displayTreeRec(currentNode->rightChild, depth + 1);
}

template<typename K, typename I>
void BST<K, I>::displayEntriesRec(Node*& currentNode)
{

    // pre order
    // std::cout << currentNode->key << std::endl;
    // if (!isLeaf(currentNode->leftChild)) displayEntriesRec(currentNode->leftChild);
    // if (!isLeaf(currentNode->rightChild)) displayEntriesRec(currentNode->rightChild);

    // in order
    if (!isLeaf(currentNode->leftChild)) displayEntriesRec(currentNode->leftChild);
    std::cout << currentNode->key << std::endl;
    if (!isLeaf(currentNode->rightChild)) displayEntriesRec(currentNode->rightChild);

    // Post order
    // if (!isLeaf(currentNode->leftChild)) displayEntriesRec(currentNode->leftChild);
    // if (!isLeaf(currentNode->rightChild)) displayEntriesRec(currentNode->rightChild);
    // std::cout << currentNode->key << std::endl;
}

template<typename K, typename I>
typename BST<K, I>::Node* BST<K, I>::detachMinimumNode(Node*& current)
{
    if (!isLeaf(current)) {
        if (isLeaf(current->leftChild)) return current; // minimumNode found
        else detachMinimumNode(current->leftChild);
    }
}

template<typename K, typename I>
void BST<K, I>::remove(KeyType key_to_remove)
{
    removeRec(root, key_to_remove);
}

template<typename K, typename I>
void BST<K, I>::removeRec(Node*& currentNode, KeyType k)
{
    if (isLeaf(currentNode)) return;
    if (currentNode->key == k) {

        if (isLeaf(currentNode->leftChild) && isLeaf(currentNode->rightChild)) {
            delete currentNode;
            currentNode = leaf();
        }

        else if (isLeaf(currentNode->leftChild)) {
            Node* temp = currentNode;
            currentNode = currentNode->rightChild;
            delete temp;
        }

        else if (isLeaf(currentNode->rightChild)) {
            Node* temp = currentNode;
            currentNode = currentNode->leftChild;
            delete temp;
        }

        else if (!isLeaf(currentNode->leftChild) && !isLeaf(currentNode->rightChild)) {

            Node* minimumNode = detachMinimumNode(currentNode->rightChild);
            currentNode->key = minimumNode->key;
            currentNode->item = minimumNode->item;

            if (!isLeaf(minimumNode->rightChild)) {
                minimumNode = minimumNode->rightChild;
                delete minimumNode->rightChild;
            }
        }
    }
    else if (currentNode->key > k) removeRec(currentNode->leftChild, k);
    else if (currentNode->key < k) removeRec(currentNode->rightChild, k);
}

template<typename K, typename I>
void  BST<K, I>::removeIf(std::function<bool(KeyType)> condition) {
    removeIfRec(condition, root);
}

template<typename K, typename I>
void BST<K, I>::removeIfRec(std::function<bool(KeyType)> condition, Node*& currentNode) {
    if (isLeaf(currentNode)) return;
    if (!isLeaf(currentNode->leftChild)) removeIfRec(condition, currentNode->leftChild);
    if (!isLeaf(currentNode->rightChild)) removeIfRec(condition, currentNode->rightChild);
    if (condition(currentNode->key)) remove(currentNode->key);
}

template<typename K, typename I>
BST<K, I>::~BST()
{
    deepDelete(root);
}

template<typename K, typename I>
void BST<K, I>::deepDelete(Node* currentNode)
{
    if (isLeaf(currentNode)) return;
    deepDelete(currentNode->leftChild);
    deepDelete(currentNode->rightChild);
    delete currentNode;
    currentNode = leaf();
}

template<typename K, typename I>
BST<K, I>::BST(const BST& bstToCopy)
{
    this->root = deepCopy(bstToCopy.root);
}

template<typename K, typename I>
typename BST<K, I>::Node* BST<K, I>::deepCopy(Node* nodeToCopy)
{
    if (isLeaf(nodeToCopy)) return nullptr;
    Node* copiedNode = new Node(nodeToCopy->key, nodeToCopy->item);
    copiedNode->leftChild = deepCopy(nodeToCopy->leftChild);
    copiedNode->rightChild = deepCopy(nodeToCopy->rightChild);
    return copiedNode;
}

template<typename K, typename I>
BST<K, I>& BST<K, I>::operator=(const BST<K, I>& bstToCopy)
{
    if (this == &bstToCopy) return *this;
    deepDelete(this->root);
    this->root = deepCopy(bstToCopy.root);
    return *this;
}

template<typename K, typename I>
BST<K, I>::BST(BST<K, I>&& bstToMove)
{
    // code to steal the nodes from 'bstToMove' goes here
    deepDelete(this->root);
    this->root = bstToMove.root;
    bstToMove.root = leaf();
}

template<typename K, typename I>
BST<K, I>& BST<K, I>::operator=(BST<K, I>&& bstToMove)
{
    // code to steal the nodes from 'bstToMove' goes here
    if (this == &bstToMove) return *this;
    deepDelete(this->root);
    this->root = bstToMove.root;
    bstToMove.root = leaf();
    return *this;
}

template<typename K, typename I>
void BST<K, I>::rotateTest()
{
    rotateRight(root);
}

template<typename K, typename I>
void BST<K, I>::rotateLTest()
{
    rotateLeft(root);
}

template<typename K, typename I>
void BST<K, I>::rotateRight(Node*& localRoot)
{
    assert(!isLeaf(localRoot));
    Node* a = localRoot->leftChild;
    assert(!isLeaf(a));
    Node* beta = a->rightChild;

    //Right rotation
    localRoot->leftChild = beta;
    a->rightChild = localRoot;
    localRoot = a;
}

template<typename K, typename I>
void BST<K, I>::rotateLeft(Node*& localRoot)
{
    assert(!isLeaf(localRoot));
    Node* b = localRoot->rightChild;
    assert(!isLeaf(b));
    Node* beta = b->leftChild;

    //Left rotation
    localRoot->rightChild = beta;
    b->leftChild = localRoot;
    localRoot = b;
}

//BST::ItemType* BST::lookup(KeyType soughtKey)
//{
//    Node* currentNode = root;
//    while (!isLeaf(currentNode)){
//        if (currentNode->key == soughtKey){
//            return &(currentNode->item);
//        }
//        else if (currentNode->key > soughtKey){
//            currentNode = currentNode->leftChild;
//        }
//        else if (currentNode->key < soughtKey){
//            currentNode = currentNode->rightChild;
//        }
//    }
//}


/*

template<typename K, typename I>
class HashTable
{
public:
    using KeyType = std::string;
    using ItemType = std::string;

    struct Node;
    HashTable();

    void insert(KeyType, ItemType);
    ItemType* lookup(KeyType);
    void remove(KeyType);

private:
    unsigned int hash(KeyType);
    void removeRec(Node*&, KeyType);
};

template<typename K, typename I>
void HashTable<K, I>::remove(KeyType key_to_remove)
{
    removeRec(root, key_to_remove);
}

template<typename K, typename I>
void HashTable<K, I>::removeRec(Node*& currentNode, KeyType k)
{
    if (isLeaf(currentNode)) return;
    if (currentNode->key == k) {

        if (isLeaf(currentNode->leftChild) && isLeaf(currentNode->rightChild)) {
            delete currentNode;
            currentNode = leaf();
        }

        else if (isLeaf(currentNode->leftChild)) {
            Node* temp = currentNode;
            currentNode = currentNode->rightChild;
            delete temp;
        }

        else if (isLeaf(currentNode->rightChild)) {
            Node* temp = currentNode;
            currentNode = currentNode->leftChild;
            delete temp;
        }

        else if (!isLeaf(currentNode->leftChild) && !isLeaf(currentNode->rightChild)) {

            Node* minimumNode = detachMinimumNode(currentNode->rightChild);
            currentNode->key = minimumNode->key;
            currentNode->item = minimumNode->item;

            if (!isLeaf(minimumNode->rightChild)) {
                minimumNode = minimumNode->rightChild;
                delete minimumNode->rightChild;
            }
        }
    }
    else if (currentNode->key > k) removeRec(currentNode->leftChild, k);
    else if (currentNode->key < k) removeRec(currentNode->rightChild, k);
}

*/


#endif BST_H
