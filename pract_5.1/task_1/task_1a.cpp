#include <iostream>
#include <bitset>

using namespace std;

int main() {
    setlocale(0, "");
    unsigned char x = 255; // 11111111
    unsigned char maska = 1; // 00000001

    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;
    x = x & (~(maska << 4));
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;

    x = 127; // 01111111
    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;
    x = x & (~(maska << 4));
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;

    x = 64; // 01000000
    cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;
    x = x & (~(maska << 4));
    cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;

    return 0;
}
