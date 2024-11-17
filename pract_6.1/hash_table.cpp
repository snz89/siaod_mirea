#include "hash_table.h"

#include <iostream>

using namespace std;

/**
 * @brief Конструктор хеш-таблицы с заданным размером.
 * 
 * Инициализирует хеш-таблицу с указанным размером. По умолчанию размер составляет 10.
 * 
 * @param size Размер хеш-таблицы.
 */
HashTable::HashTable(int size) {
        this->size = size;
        this->count = 0;
        this->table.resize(size);
}

/**
 * @brief Хеш-функция для вычисления индекса.
 * 
 * Вычисляет индекс для ключа на основе его значения и размера хеш-таблицы.
 * 
 * @param key Ключ для хеширования.
 * @return Индекс в хеш-таблице, соответствующий данному ключу.
 */
int HashTable::hashFunction(int key) {
    return key % this->size;
}

/**
 * @brief Рехеширование хеш-таблицы.
 * 
 * Увеличивает размер хеш-таблицы вдвое и перераспределяет все элементы.
 */
void HashTable::rehash() {
    int newSize = size * 2;
    vector<list<BankAccount>> newTable(newSize);

    for (int i = 0; i < size; ++i) {
        for (const auto& account : table[i]) {
            int newIndex = account.id % newSize;
            newTable[newIndex].push_back(account);
        }
    }

    this->size = newSize;
    this->table = move(newTable);
}

/**
 * @brief Вычисляет коэффициент загрузки хеш-таблицы.
 * 
 * Коэффициент загрузки определяется как отношение количества элементов 
 * к текущему размеру хеш-таблицы.
 * 
 * @return Коэффициент загрузки хеш-таблицы.
 */
double HashTable::loadFactor() const {
    return (double)count / size;
}

/**
 * @brief Вставка нового элемента в хеш-таблицу.
 * 
 * Добавляет новую запись в хеш-таблицу. Если коэффициент загрузки превышает 0.75, 
 * выполняется перехеширование для увеличения размера таблицы.
 * Если элемент с таким же ключом уже существует, вставка не происходит.
 * 
 * @param key Ключ записи (уникальный идентификатор).
 * @param fullName Полное имя владельца счета.
 * @param address Адрес владельца счета.
 * @return true, если элемент успешно добавлен, false — если элемент с таким ключом уже существует.
 */
bool HashTable::insert(int key, const string& fullName, const string& address) {
    if (loadFactor() >= 0.75) {
        rehash();
    }

    int index = hashFunction(key);

    for (const auto& account : table[index]) {
        if (account.id == key) {
            return false;
        }
    }

    table[index].push_back({key, fullName, address});
    this->count++;
    return true;
}

/**
 * @brief Удаление элемента из хеш-таблицы по ключу.
 * 
 * Ищет элемент с указанным ключом и удаляет его из хеш-таблицы. Если элемент найден, 
 * его удаляют, и количество элементов уменьшается. Если элемент с таким ключом не найден, 
 * возвращается false.
 * 
 * @param key Ключ элемента, который нужно удалить.
 * @return true, если элемент был успешно удален, false — если элемент с таким ключом не найден.
 */
bool HashTable::remove(int key) {
    int index = hashFunction(key);

    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->id == key) {
            table[index].erase(it);
            this->count--;
            return true;
        }
    }

    return false; // Элемент с таким ключом не найден
}

/**
 * @brief Поиск элемента в хеш-таблице по ключу.
 * 
 * Ищет элемент с указанным ключом в хеш-таблице. Если элемент найден, возвращает указатель 
 * на соответствующую запись. Если элемент с таким ключом не найден, возвращает nullptr.
 * 
 * @param key Ключ элемента, который нужно найти.
 * @return Указатель на элемент с указанным ключом, или nullptr, если элемент не найден.
 */
BankAccount* HashTable::search(int key) {
    int index = hashFunction(key);

    for (auto& account : table[index]) {
        if (account.id == key) {
            return &account;
        }
    }

    return nullptr;
}

/**
 * @brief Вывод хеш-таблицы на экран.
 * 
 * Печатает содержимое хеш-таблицы, выводя для каждой ячейки таблицы ее индекс и все записи, 
 * которые в ней хранятся. Если ячейка пуста, выводится "{}".
 */
void HashTable::print() const {
    for (int i = 0; i < size; i++) {
        cout << "[" << i << "]: ";
        if (table[i].empty()) {
            cout << "{}\n";
        } else {
            for (const auto& account : table[i]) {
                cout << "{ID: " << account.id
                        << ", Name: " << account.fullName
                        << ", Address: " << account.address << "} ";
            }
            cout << "\n";
        }
    }
}
