#include <iostream>
#include <bitset>

using namespace std;

int main() {
    setlocale(0, "");
    unsigned long long bitArray = 0;
    int num;

    cout << "Введите числа от 0 до 63: ";
    while (true) {
        cin >> num;
        if (num == -1)
            break;
        if (num >= 0 && num <= 63) {
            bitArray |= (1ULL << num);
        }
        else {
            cout << "Неверный ввод. Введите число от 0 до 63" << endl;
        }
    }

    cout << "Отсортированные числа: ";
    for (int i = 0; i < 64; ++i) {
        if ((bitArray >> i) & 1) {
            cout << i << " ";
        }
    }
    cout << endl;

    return 0;
}

