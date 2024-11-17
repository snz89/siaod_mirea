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

constexpr int minCarNumber = 10000;
constexpr int maxCarNumber = 99999;
constexpr int maxNumberCount = maxCarNumber - minCarNumber + 1;

/**
 * Запись о владельце авто
 * @param carNumber Номер автомобиля.
 * @param carBrand Марка автомобиля.
 * @param fullName Полное имя владельца.
 */
struct CarOwner {
    int carNumber;
    char carBrand[16];
    char fullName[56];
};

/**
 * Запись пары ключ-смещение для таблицы смещений.
 * @param key Ключ
 * @param offset Смещение.
 */
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

/**
 * Генерация текстового файла с информацией о владельцах авто.
 * @param fileName Имя файла.
 * @param numRecords Количество генерируемых записей.
 */
void generateTextFile(const char* fileName, int numRecords) {
    const char carBrands[][16] = {
        "Toyota", "Honda", "Ford", "BMW", "Audi", "Mercedes",
        "Volkswagen", "Chevrolet", "Nissan", "Hyundai"
    };
    const int brandCount = sizeof(carBrands) / sizeof(carBrands[0]);

    // Создание вектора с уникальными id
    vector<int> numbers(maxNumberCount);
    iota(numbers.begin(), numbers.end(), minCarNumber);

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
        const char* carBrand = carBrands[g() % brandCount];
        textFile << id << " " << carBrand << " " << "Name_" << i + 1 << "\n";
    }

    textFile.close();
}

/**
 * Конвертация информации о студентах в бинарный файл
 * @param textFile Имя входного текстового файла.
 * @param binaryFile Имя выходного бинарного файла.
 */
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

    CarOwner record;

    while (text >> record.carNumber >> record.carBrand) {
        text.ignore();
        text.getline(record.fullName, sizeof(record.fullName));
        binary.write((char*)(&record), sizeof(record));
    }
}

/**
* Линейный поиск по ключу.
* @param binaryFile Имя входного бинарного файла
* @param searchKey Ключ для поиска.
* @return CarOwner* Указатель на копию найденной записи.
* @warning Если запись не найдена, возвращается nullptr
*/
CarOwner* linearSearch(const char* binaryFile, int searchKey) {
    ifstream binary(binaryFile, ios::binary);

    if (!binary) {
        cerr << "File opening error " << binaryFile << "\n";
        return nullptr;
    }

    CarOwner record;

    while (binary.read((char*)(&record), sizeof(record))) {
        if (record.carNumber == searchKey) {
            return new CarOwner(record);
        }
    }

    binary.close();
    return nullptr;
}

/**
 * Генерация таблицы со смещениями на записи.
 * @param binaryFile Имя входного бинарного файла.
 * @return vector<OffsetRecord> Вектор смещений на записи.
 */
vector<OffsetRecord> createOffsetTable(const char* binaryFile) {
    ifstream binary(binaryFile, ios::binary);

    if (!binary) {
        cerr << "File opening error " << binaryFile << "\n";
        return {};
    }

    vector<OffsetRecord> offsetTable;
    CarOwner record;
    long offset = 0;
    
    while (binary.read((char*)(&record), sizeof(record))) {
        offsetTable.push_back({record.carNumber, offset});
        offset += sizeof(CarOwner);
    }

    binary.close();

    sort(offsetTable.begin(), offsetTable.end());
    
    return offsetTable;
}

/**
 * Генерация таблицы смещений для однородного бинарного поиска.
 * @param tableSize Размер таблицы со смещениями на записи.
 * @return vector<int> Вектор смещений.
 */
vector<int> generateDeltaTable(int tableSize) {
    vector<int> deltaTable;
    int j = 1;
    while (floor((tableSize + pow(2, j - 1)) / pow(2, j)) > 0) {
        deltaTable.push_back(floor((tableSize + pow(2, j - 1)) / pow(2, j)));
        j++;
    }
    return deltaTable;
}

/**
 * Однородный бинарный поиск с таблицей смещений.
 * @param key Искомый ключ.
 * @param offsetTable Таблица со смещениями на записи.
 * @return long Смещение найденной записи или -1, если запись не найдена.
 */
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

/**
 * Вычисляет число Фибоначчи по индексу.
 * @param n Индекс числа Фибоначчи.
 * @return long Число Фибоначчи.
 */
long fibonacci(int n) {
    if (n == 0 || n == 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

/**
 * Поиск Фибонначи по таблице смещений
 * @param key Искомый ключ.
 * @param offsetTable Таблица со смещениями на записи.
 * @return long Смещение найденной записи или -1, если запись не найдена.
 */
long fibonacciSearch(int key, const vector<OffsetRecord>& offsetTable) {
    int N = offsetTable.size();
    if (N == 0) {
        return -1;
    }

    // Инициализация чисел Фибоначчи
    int fibMm2 = 0; // (m-2)'e число Фибоначчи
    int fibMm1 = 1; // (m-1)'e число Фибоначчи
    int fibM = fibMm2 + fibMm1; // m'е число Фибоначчи

    // Находим минимальное m, для которого fibM >= N
    while (fibM < N) {
        fibMm2 = fibMm1;
        fibMm1 = fibM;
        fibM = fibMm2 + fibMm1;
    }

    // Смещение для проверки элементов с конца
    int offset = -1;

    while (fibM > 1) {
        // Проверяем индекс
        int i = min(offset + fibMm2, N - 1);

        // Сравниваем ключ с ключом на позиции i
        if (offsetTable[i].key < key) {
            // Сдвигаем окно поиска вправо
            fibM = fibMm1;
            fibMm1 = fibMm2;
            fibMm2 = fibM - fibMm1;
            offset = i;
        } else if (offsetTable[i].key > key) {
            // Сдвигаем окно поиска влево
            fibM = fibMm2;
            fibMm1 = fibMm1 - fibMm2;
            fibMm2 = fibM - fibMm1;
        } else {
            return offsetTable[i].offset; // Найдено
        }
    }

    // Проверяем последний возможный элемент
    if (fibMm1 && offsetTable[offset + 1].key == key) {
        return offsetTable[offset + 1].offset;
    }

    return -1; // Не найдено
}

/**
 * Поиск информации о студенте по смещению.
 * @param offset смещение
 * @param binaryFile имя бинарного файла.
 * @return Указатель на копию записи о студенте или nullptr, если запись не найдена.
 */
CarOwner* searchRecordByOffset(long offset, const char* binaryFile) {
    ifstream binary(binaryFile, ios::binary);

    if (!binary) {
        cerr << "File opening error " << binaryFile << "\n";
        return nullptr;
    }

    binary.seekg(offset);
    CarOwner record;
    binary.read((char*)&record, sizeof(CarOwner));
    binary.close();

    return new CarOwner(record);
}

void Task2(int searchKey) {
    auto start = chrono::high_resolution_clock::now();

    CarOwner* result = linearSearch("data.bin", searchKey);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    if (result) {
        cout << "Found: "
            << result->carNumber << " "
            << result->carBrand << " "
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
    
    // TODO: Заменить однородный бинарный поиск на Фибоначчи поиск
    long offset = fibonacciSearch(searchKey, offsetTable);

    if (offset != -1) {
        CarOwner* result = searchRecordByOffset(offset, "data.bin");
        cout << "Found: "
            << result->carNumber << " "
            << result->carBrand << " "
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
    // generateTextFile("data.txt", 100);
    // textToBinary("data.txt", "data.bin");

    int searchKey = 29944;

    cout << "Task 2\n";
    Task2(searchKey);
    cout << "\nTask 3\n";
    Task3(searchKey);
    return 0;
}