#include "bst.h"
#include <cassert>

BST::Node* BST::leaf()
{
    return nullptr;
}

bool BST::isLeaf(Node* n)
{
    return (n == leaf());
}

struct BST::Node
{
    Node(KeyType, ItemType);
    KeyType key;
    ItemType item;
    Node* leftChild;
    Node* rightChild;
};

BST::Node::Node(KeyType key, ItemType item)
{
    this->key = key;
    this->item = item;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}


BST::ItemType* BST::lookup(KeyType soughtKey)
{
    return lookupRec(soughtKey, root);
}


BST::ItemType* BST::lookupRec(KeyType soughtKey, Node* currentNode)
{
    if (isLeaf(currentNode)) return nullptr;
    else if (soughtKey == currentNode->key) return &(currentNode->item);
    else if (currentNode->key > soughtKey) return lookupRec(soughtKey, currentNode->leftChild);
    else if (currentNode->key < soughtKey) return lookupRec(soughtKey, currentNode->rightChild);
}


void BST::insert(KeyType k, ItemType i)
{
    insertRec(k, i, root);
}

void BST::insertRec(KeyType k, ItemType i, Node* & currentNode)
{
    if (isLeaf(currentNode)) currentNode = new Node(k, i);
    else if (currentNode->key == k) currentNode->item = i;
    else if (currentNode->key > k) insertRec(k, i, currentNode->leftChild);
    else if (currentNode->key < k) insertRec(k, i, currentNode->rightChild);
}

void BST::displayEntries()
{
    displayEntriesRec(root);
}

void BST::displayTree()
{
    displayTreeRec(root, 0);
}

void BST::displayTreeRec(Node*& currentNode, int depth)
{
    std::string tabs;
    for (size_t i = 0; i < depth; ++i) tabs += "\t";
    std::cout << tabs << currentNode->key << std::endl;
    if (!isLeaf(currentNode->leftChild)) displayTreeRec(currentNode->leftChild, depth + 1);
    if (!isLeaf(currentNode->rightChild)) displayTreeRec(currentNode->rightChild, depth + 1);
}

void BST::displayEntriesRec(Node*& currentNode)
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

BST::Node* BST::detachMinimumNode(Node*& current)
{
    if (!isLeaf(current)) {
        if (isLeaf(current->leftChild)) return current; // minimumNode found
        else detachMinimumNode(current->leftChild);
    }
}

void BST::remove(KeyType key_to_remove)
{
    removeRec(root, key_to_remove);
}

BST::Node* BST::removeRec(Node*& currentNode, KeyType k)
{
    if (isLeaf(currentNode)) return root;

    if (currentNode->key == k) {

        if (isLeaf(currentNode->leftChild) && isLeaf(currentNode->rightChild)) {
            delete currentNode->leftChild;
            delete currentNode->rightChild;
            currentNode = leaf();
        }

        else if (isLeaf(currentNode->leftChild) && !isLeaf(currentNode->rightChild)) {
            delete currentNode->leftChild;
            currentNode = currentNode->rightChild;
            delete currentNode->rightChild;
        }

        else if (!isLeaf(currentNode->leftChild) && isLeaf(currentNode->rightChild)) {
            delete currentNode->rightChild;
            currentNode = currentNode->leftChild;
            delete currentNode->leftChild;
        }

        else if (!isLeaf(currentNode->leftChild) && !isLeaf(currentNode->rightChild)) {
            Node* minimumNode = detachMinimumNode(currentNode->rightChild);
            currentNode->key = minimumNode->key;
            currentNode->item = minimumNode->item;
           
            if (!isLeaf(minimumNode->rightChild)) {
                Node* temp = minimumNode;
                delete minimumNode->rightChild;
                minimumNode = temp->rightChild;
                delete temp;
            }
        }
    }

    else if (currentNode->key > k) removeRec(currentNode->leftChild, k);
    else if (currentNode->key < k) removeRec(currentNode->rightChild, k);
}

void BST::removeIf(std::function<bool(KeyType)>) {

}

BST::~BST()
{
    deepDelete(root);
}


void BST::deepDelete(Node*& currentNode)
{
    if (isLeaf(currentNode)) return;
    if (!isLeaf(currentNode->leftChild)) deepDelete(currentNode->leftChild);
    if (!isLeaf(currentNode->rightChild)) deepDelete(currentNode->rightChild);
    delete currentNode;
}

BST::BST(const BST& bstToCopy)
{
    this->root = deepCopy(bstToCopy.root);
}

BST::Node* BST::deepCopy(Node* nodeToCopy)
{
    if (isLeaf(nodeToCopy)) return nullptr;
    Node* copiedNode = new Node(nodeToCopy->key, nodeToCopy->item);
    copiedNode->leftChild = deepCopy(nodeToCopy->leftChild);
    copiedNode->rightChild = deepCopy(nodeToCopy->rightChild);
    return copiedNode;
}

BST& BST::operator=(const BST& bstToCopy)
{
    if (this == &bstToCopy) return *this;
    deepDelete(this->root);
    this->root = deepCopy(bstToCopy.root);
    return *this;
}

BST::BST(BST&& bstToMove)
{
    // code to steal the nodes from 'bstToMove' goes here
    deepDelete(this->root);
    this->root = bstToMove.root;
    bstToMove.root = leaf();
}

BST& BST::operator=(BST&& bstToMove)
{
    // code to steal the nodes from 'bstToMove' goes here
    if (this == &bstToMove) return *this;
    deepDelete(this->root);
    this->root = bstToMove.root;
    bstToMove.root = leaf();
    return *this;
}

void BST::rotateTest()
{
    rotateRight(root);
}

void BST::rotateLTest()
{
    rotateLeft(root);
}

void BST::rotateRight(Node*& localRoot)
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

void BST::rotateLeft(Node*& localRoot)
{
    assert(!isLeaf(localRoot));
    Node* b = localRoot->rightChild;
    assert(!isLeaf(b));
    Node* beta = b->leftChild;

    //Right rotation
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