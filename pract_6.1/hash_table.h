#ifndef HASH_TABLE
#define HASH_TABLE

#include "book.h"
#include <list>
#include <vector>

using namespace std;

class HashTable {
private:
    vector<list<Book>> table;
    int size;
    int count;
    int hashFunction(unsigned long long key);
    void rehash();
    double loadFactor() const;
public:
    HashTable(int size = 10);
    bool insert(unsigned long long key, const string& fullName, const string& address);
    bool remove(unsigned long long key);
    Book* search(unsigned long long key);
    void print() const;
};

#endif
