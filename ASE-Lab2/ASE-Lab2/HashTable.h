#pragma once
#include <iomanip>

class HashTable
{
public:
    using KeyType = std::string;
    using ItemType = std::string;

    HashTable();

    void insert(KeyType, ItemType);
    ItemType* lookup(KeyType);
    void remove(KeyType);

private:
    unsigned int hash(KeyType);
};