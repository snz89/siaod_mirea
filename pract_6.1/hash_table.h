#ifndef HASH_TABLE
#define HASH_TABLE

#include "bank_account.h"
#include <list>
#include <vector>

using namespace std;

class HashTable {
private:
    vector<list<BankAccount>> table;
    int size;
    int count;
    int hashFunction(int key);
    void rehash();
    double loadFactor() const;
public:
    HashTable(int size = 10);
    bool insert(int key, const string& fullName, const string& address);
    bool remove(int key);
    BankAccount* search(int key);
    void print() const;
};

#endif
