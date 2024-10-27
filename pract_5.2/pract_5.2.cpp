#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

using namespace std;

// Запись о студенте
struct Record {
    int passbookId;
    int groupId;
    char fullName[56];
};

// Запись пары ключ-смещение для таблицы смещений
struct OffsetRecord {
    int key;
    long offset;

    bool operator<(const OffsetRecord& other) const {
        if (key != other.key) {
            return key < other.key;
        }
        return false;
    }
};

// Генерация текстового файла с информацией о студентах
// Предусловие: fileName - имя файла, numRecords - количество записей
// Постусловие: создан текстовый файл с numRecords записей о студентах, 
// номера зачеток уникальны и находятся в диапазоне [10000, 99999]
void generateTextFile(const char* fileName, int numRecords) {
    const int minPassbookId = 10000;
    const int maxPassbookId = 99999;
    const int maxIdCount = maxPassbookId - minPassbookId + 1;

    // Создание вектора с уникальными id
    vector<int> numbers(maxIdCount);
    iota(numbers.begin(), numbers.end(), minPassbookId);

    random_device rd;
    mt19937 g(rd());
    shuffle(numbers.begin(), numbers.end(), g);

    numbers.resize(numRecords);

    ofstream textFile(fileName);
    if (!textFile) {
        cerr << "File opening error: " << fileName << "\n";
        return;
    }

    for (int i = 0; i < numRecords; i++) {
        int id = numbers[i];
        int groupId = id % 50 + 10;
        textFile << id << " " << groupId << " " << "Name_" << i + 1 << "\n";
    }

    textFile.close();
}

// Конвертация информации о студентах в бинарный файл
// Предусловие: textFile - имя текстового файла, binaryFile - имя бинарного файла
// Постусловие: создан бинарный файл, содержащий записи о студентах из textFile
void textToBinary(const char* textFile, const char* binaryFile) {
    // Открытие файлов
    ifstream text(textFile);
    ofstream binary(binaryFile, ios::binary);

    if (!text) {
        cerr << "File opening error " << textFile << "\n";
        return;
    }

    if (!binary) {
        cerr << "File opening error " << binaryFile << "\n";
        return;
    }

    Record record;

    while (text >> record.passbookId >> record.groupId) {
        text.ignore();
        text.getline(record.fullName, sizeof(record.fullName));
        binary.write((char*)(&record), sizeof(record));
    }
}

// Линейный поиск студента по ключу
// Предусловие: binaryFile - имя существующего бинарного файла, 
// searchKey - ключ для поиска
// Постусловие: возвращен указатель на копию найденной записи 
// или nullptr, если запись не найдена. Файл закрыт.
Record* linearSearch(const char* binaryFile, int searchKey) {
    ifstream binary(binaryFile, ios::binary);

    if (!binary) {
        cerr << "File opening error " << binaryFile << "\n";
        return nullptr;
    }

    Record record;

    while (binary.read((char*)(&record), sizeof(record))) {
        if (record.passbookId == searchKey) {
            return new Record(record);
        }
    }

    binary.close();
    return nullptr;
}

// Генерация таблицы со смещениями
vector<OffsetRecord> createOffsetTable(const char* binaryFile) {
    ifstream binary(binaryFile, ios::binary);

    if (!binary) {
        cerr << "File opening error " << binaryFile << "\n";
        return {};
    }

    vector<OffsetRecord> offsetTable;
    Record record;
    long offset = 0;
    
    while (binary.read((char*)(&record), sizeof(record))) {
        offsetTable.push_back({record.passbookId, offset});
        offset += sizeof(Record);
    }

    binary.close();

    sort(offsetTable.begin(), offsetTable.end());
    
    return offsetTable;
}

// Генерация таблицы смещений для однородного бинарного поиска
// Предусловие: размер таблицы со смещениями на записи
// Постусловие: таблицы со смещениями для поиска
vector<int> generateDeltaTable(int tableSize) {
    vector<int> deltaTable;
    int j = 1;
    while (floor((tableSize + pow(2, j - 1)) / pow(2, j)) > 0) {
        deltaTable.push_back(floor((tableSize + pow(2, j - 1)) / pow(2, j)));
        j++;
    }
    return deltaTable;
}

// Однородный бинарный поиск с таблицей смещений
// Предусловие: искомый ключ, таблица со смещениями на записи
// Постусловие: смещение на запись
long searchOffset(int key, const vector<OffsetRecord>& offsetTable) {
    int tableSize = offsetTable.size();
    if (tableSize == 0) {
        return -1;
    }

    // Генерируем таблицу смещений
    vector<int> deltaTable = generateDeltaTable(tableSize);

    int currentPosition = deltaTable[0];
    int deltaIndex = 0;

    // Имя-сторож для четного n
    if (tableSize % 2 == 0 && key < offsetTable[0].key) {
        return -1;
    }

    while (deltaIndex < deltaTable.size()) {
        if (offsetTable[currentPosition].key == key) {
            return offsetTable[currentPosition].offset;
        }
        else if (offsetTable[currentPosition].key < key) {
            currentPosition += deltaTable[++deltaIndex];
            if (currentPosition >= tableSize)
                currentPosition = tableSize - 1;
        }
        else {
            currentPosition -= deltaTable[++deltaIndex];
            if (currentPosition < 0)
                currentPosition = 0;
        }
    }

    // Последняя проверка
    if (offsetTable[currentPosition].key == key) {
        return offsetTable[currentPosition].offset;
    }

    return -1;
}

// Поиск информации о студенте по смещению
// Предусловие: смещение на запись, название бинарного файла
// Постусловие: запись о студенте (Record)
Record* searchRecordByOffset(long offset, const char* binaryFile) {
    ifstream binary(binaryFile, ios::binary);

    if (!binary) {
        cerr << "File opening error " << binaryFile << "\n";
        return nullptr;
    }

    binary.seekg(offset);
    Record record;
    binary.read((char*)&record, sizeof(Record));
    binary.close();

    return new Record(record);
}

void Task2(int searchKey) {
    auto start = chrono::high_resolution_clock::now();

    Record* result = linearSearch("data.bin", searchKey);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    if (result) {
        cout << "Found: "
            << result->passbookId << " "
            << result->groupId << " "
            << result->fullName << "\n";
        delete result;
    }
    else {
        cout << "Key " << searchKey << " is not found\n";
    }

    cout << "Time: " << duration.count() << "\n";
}

void Task3(int searchKey) {
    // Генерируем таблицу смещений
    vector<OffsetRecord> offsetTable = createOffsetTable("data.bin");

    auto start = chrono::high_resolution_clock::now();

    long offset = searchOffset(searchKey, offsetTable);

    if (offset != -1) {
        Record* result = searchRecordByOffset(offset, "data.bin");
        cout << "Found: "
            << result->passbookId << " "
            << result->groupId << " "
            << result->fullName << "\n";
        delete result;
    }
    else {
        cout << "Key " << searchKey << " is not found\n";
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time: " << duration.count() << "\n";
}

int main() {
    // Task 1
    generateTextFile("data.txt", 100);
    textToBinary("data.txt", "data.bin");

    int searchKey = 37600;

    Task2(searchKey);
    Task3(searchKey);
    return 0;
}