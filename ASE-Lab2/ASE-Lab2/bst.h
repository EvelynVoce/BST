#pragma once

#ifndef BST_H
#define BST_H

#include <iostream>
#include <functional>

class BST
{
public:
    BST() = default; // Constructor
    BST(const BST&); // Copy constructor
    BST& operator=(const BST&); // Copy assignment constructor
    BST(BST&&); // Move constructor
    BST& operator=(BST&&); //Move assignment constructor
    ~BST(); // Destructor
    using KeyType = int;
    using ItemType = std::string;
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
    Node* detachMinimumNode(Node*&);
    void deepDelete(Node*); // recursive worker performing deep delete
    Node* deepCopy(Node*);
    void rotateLeft(Node*&);
    void rotateRight(Node*&);
};

#endif // BST_H
