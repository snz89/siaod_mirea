#include <iostream>
#include <vector>

using namespace std;

int main() {
    setlocale(0, "");
    vector<unsigned char> bitArray(8);
    int num;

    cout << "Введите числа от 0 до 63: ";
    while (true)
    {
        cin >> num;
        if (num == -1)
            break;
        if (num >= 0 && num <= 63) {
            int byteIndex = num / 8;
            int bitIndex = num % 8;
            bitArray[byteIndex] |= (1 << bitIndex);
        }
        else {
            cout << "Неверный ввод. Введите число от 0 до 63" << endl;
        }
    }

    cout << "Отсортированные числа: ";
    for (int i = 0; i < 64; ++i)
    {
        int byteIndex = i / 8;
        int bitIndex = i % 8;
        if ((bitArray[byteIndex] >> bitIndex) & 1)
        {
            cout << i << " ";
        }
    }
    cout << endl;

    return 0;
}

