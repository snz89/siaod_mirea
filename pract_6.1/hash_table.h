#ifndef HASH_TABLE
#define HASH_TABLE

#include "bank_account.h"

class HashTable {
private:
    BankAccount** table;
    int size;
    int count;
    int hashFunction(int key);
    int hashFunction2(int key);
    void resize();
public:
    HashTable();
    HashTable(int size);
    ~HashTable();
    bool insert(int key, const string& fullName, const string& address);
    bool remove(int key);
    BankAccount* search(int key);
    void print();
};

#endif
