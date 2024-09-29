#include "task_3.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <numeric>

namespace {
    const size_t MAX_NUMBERS = 9000000; // Максимальное кол-во чисел в файле
    const size_t BIT_ARRAY_SIZE = MAX_NUMBERS / 8; // Размер битового массива в байтах
    const size_t MIN_VALUE = 1000000; // Минимальное число в файле
    const size_t MAX_VALUE = 9999999; // Максимальное число в файлк
}

void generateUniqueNumbersFile(const string& filename, size_t count) {
    if (count > (MAX_VALUE - MIN_VALUE + 1)) {
        cerr << "Ошибка: некорректный диапазон или количество чисел.\n";
        return;
    }

    vector<int> numbers(MAX_VALUE - MIN_VALUE + 1);
    iota(numbers.begin(), numbers.end(), MIN_VALUE);

    // Перемешивание вектора
    random_device rd;
    mt19937 g(rd());
    shuffle(numbers.begin(), numbers.end(), g);

    numbers.resize(count);

    ofstream outputFile(filename);
    if (outputFile.is_open()) {
        for (int number : numbers) {
            outputFile << number << "\n";
        }
        outputFile.close();
    }
    else {
        cerr << "Не удалось открыть файл для записи.\n";
        return;
    }
}

// Заполнение битового массива данными из файла
void fillBitArray(const string& filename, vector<unsigned char>& bitArray) {
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        int number;
        while (inputFile >> number) {
            number -= MIN_VALUE;
            int byteIndex = number / 8;
            int bitIndex = number % 8;
            bitArray[byteIndex] |= (1 << bitIndex);
        }
        inputFile.close();
    }
    else {
        cerr << "Не удалось открыть входной файл.\n";
        return;
    }
}

// Формирование выходного файла с отсортированными числами
void generateOutputFile(const string& filename, const vector<unsigned char> bitArray) {
    ofstream outputFile(filename);
    if (outputFile.is_open()) {
        for (int i = 0; i < BIT_ARRAY_SIZE; ++i) {
            for (int j = 0; j < 8; ++j) {
                if ((bitArray[i] >> j) & 1) {
                    outputFile << (i * 8 + j) + MIN_VALUE << "\n";
                }
            }
        }
        outputFile.close();
    }
    else {
        cerr << "Не удалось открыть выходной файл.\n";
        return;
    }
}

void Task_3(size_t numberCount) {
    // Битовый массив
    vector<unsigned char> bitArray(BIT_ARRAY_SIZE, 0);

    generateUniqueNumbersFile("input.txt", numberCount);

    auto start = chrono::high_resolution_clock::now();

    fillBitArray("input.txt", bitArray);

    generateOutputFile("output.txt", bitArray);

    // Замер времени окончания сортировки
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Сортировка завершена. Время выполнения: " << duration.count() << " мс\n";
    cout << "Размер битового массива: " << bitArray.size() / (1024.0 * 1024.0) << " МБ\n";
}