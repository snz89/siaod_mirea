#include "task_1.h"
#include <iostream>
#include <bitset>

using namespace std;

void Task_1a() {
    unsigned char x = 255; // 11111111
    unsigned char maska = 1; // 00000001

    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
    x &= (~(maska << 4));
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";

    x = 127; // 01111111
    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
    x &= (~(maska << 4));
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";

    x = 64; // 01000000
    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
    x &= (~(maska << 4));
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
}

void Task_1b() {
    unsigned char x = 127;
    unsigned char maska = 1;

    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
    x |= (maska << 6);
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";

    x = 0; // 00000000
    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
    x |= (maska << 6);
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";

    x = 59; // 00111011
    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
    x |= (maska << 6);
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << "\n";
}

void Task_1c() {
    unsigned int x = 25; // 00000000000000000000000000011001
    const int n = sizeof(int) * 8;
    cout << "Число x: " << bitset<n>(x) << "\n";
    unsigned maska = (1 << n - 1); // 10000000000000000000000000000000
    cout << "Начальный вид маски: " << bitset<n>(maska) << "\n";
    cout << "Результат: ";
    for (int i = 1; i <= n; i++) {
        cout << ((x & maska) >> (n - i)); // Вывод в десятичном представлении
        maska = maska >> 1;
    }
    cout << "\n";
}
