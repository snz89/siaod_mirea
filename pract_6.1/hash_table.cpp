#include "hash_table.h"

#include <iostream>
#include <format>

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
int HashTable::hashFunction(unsigned long long key) {
    return key % this->size;
}

/**
 * @brief Рехеширование хеш-таблицы.
 * 
 * Увеличивает размер хеш-таблицы вдвое и перераспределяет все элементы.
 */
void HashTable::rehash() {
    int newSize = size * 2;
    vector<list<Book>> newTable(newSize);

    for (int i = 0; i < size; ++i) {
        for (const auto& book : table[i]) {
            int newIndex = book.isbn % newSize;
            newTable[newIndex].push_back(book);
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
bool HashTable::insert(unsigned long long key, const string& fullName, const string& address) {
    if (loadFactor() >= 0.75) {
        rehash();
    }

    int index = hashFunction(key);

    for (const auto& book : table[index]) {
        if (book.isbn == key) {
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
bool HashTable::remove(unsigned long long key) {
    int index = hashFunction(key);

    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->isbn == key) {
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
Book* HashTable::search(unsigned long long key) {
    int index = hashFunction(key);

    for (auto& book : table[index]) {
        if (book.isbn == key) {
            return &book;
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
    // Заголовок таблицы
    cout << format("{:<5} {:<15} {:<30} {:<40}\n", "Index", "ISNB", "Author", "Title");
    cout << string(90, '-') << "\n";

    // Вывод данных
    for (int i = 0; i < size; i++) {
        if (table[i].empty()) {
            cout << format("{:<5} {:<15} {:<30} {:<40}\n", i, "-", "-", "-");
        } else {
            for (const auto& book : table[i]) {
                cout << format("{:<5} {:<15} {:<30} {:<40}\n", i, book.isbn, book.author, book.title);
            }
        }
    }
}
