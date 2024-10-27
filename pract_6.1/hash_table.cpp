#include "hash_table.h"

#include <iostream>

using namespace std;

HashTable::HashTable() {
    this->size = 10;
    this->table = new BankAccount*[10];
    this->count = 0;

    for (int i = 0; i < 10; i++) {
        this->table[i] = nullptr;
    }
}

HashTable::HashTable(int size) {
    this->size = size;
    this->table = new BankAccount*[size];
    this->count = 0;

    for (int i = 0; i < size; i++) {
        this->table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < this->size; i++) {
        if (this->table[i] != nullptr) {
            delete this->table[i];
        }
    }
    delete[] table;
}

int HashTable::hashFunction(int key) {
    return key % this->size;
}

int HashTable::hashFunction2(int key) {
    return 1 + (key % (this->size - 1));
}

// Увеличение размера таблицы в 2 раза
void HashTable::resize() {
    // Сохранение старых данных
    int formerSize = this->size;
    BankAccount** formerTable = this->table;

    // Увеличение таблицы
    this->size *= 2;
    this->table = new BankAccount*[this->size];

    for (int i = 0; i < this->size; i++) {
        this->table[i] = nullptr;
    }
    this->count = 0;

    // Рехеширование в соответстсвии с новым размером таблицы
    for (int i = 0; i < formerSize; i++) {
        if (formerTable[i] != nullptr && !formerTable[i]->deleted) {
            this->insert(formerTable[i]->id, formerTable[i]->fullName, formerTable[i]->address);
            delete formerTable[i]; // Освобождение памяти для удаленных элементов
        }
    }

    delete[] formerTable;
}

bool HashTable::insert(int key, const string& fullName, const string& address) {
    if (this->search(key) != nullptr) {
        return false;
    }
    // Проверка на загрузку
    if (this->count >= this->size * 0.7) {
        this->resize();
    }

    BankAccount* newAccount = new BankAccount{key, fullName, address};

    int index = this->hashFunction(key);
    int step = this->hashFunction2(key);

    // Поиск открытой ячейки
    while (this->table[index] != nullptr && !this->table[index]->deleted) {
        index = (index + step) % this->size;
    }

    table[index] = newAccount;
    this->count++;
    return true;
}

bool HashTable::remove(int key) {
    BankAccount* account = this->search(key);

    if (account != nullptr && !account->deleted) {
        account->deleted = true;
        this->count--;
        return true;
    }
    return false;
}

BankAccount* HashTable::search(int key) {
    int index = this->hashFunction(key);
    int step = this->hashFunction2(key);

    // Поиск элемента среди закрытых ячеек
    while (this->table[index] != nullptr) {
        if (this->table[index]->id == key && !this->table[index]->deleted) {
            return table[index];
        }
        index = (index + step) % this->size;
    }

    return nullptr;
}

void HashTable::print() {
    // Проверка на пустоту
    if (count == 0) {
        cout << "[] There is no data\n";
        return;
    }
    for (int i = 0; i < this->size; i++) {
        if (this->table[i] != nullptr && !this->table[i]->deleted) {
            cout << "[" << i << "] "
            << "ID: " << this->table[i]->id
            << ", Full Name: " << this->table[i]->fullName
            << ", Address: " << this->table[i]->address << "\n";
        }
    }
}
