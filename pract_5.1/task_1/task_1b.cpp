    #include <iostream>
    #include <bitset>

    using namespace std;

    int main() {
        setlocale(0, "");
        unsigned char x = 127;
        unsigned char maska = 1; 

        cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;
        x = x | (maska << 6);
        cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;

        x = 0; // 00000000
        cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;
        x = x | (maska << 6);
        cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;

        x = 59; // 11111111
        cout << "Исходное значение x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;
        x = x | (maska << 6);
        cout << "Результат x: " << (int)x << " (" << bitset<8>(x) << ")" << endl;

        return 0;
    }

