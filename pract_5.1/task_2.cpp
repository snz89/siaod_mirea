#include "task_2.h"
#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

void Task_2a() {
    unsigned char bitArray = 0;
    int num, count;

    cout << "������� ���������� ����� (�� 1 �� 8): ";
    cin >> count;

    if (count < 1 || count > 8) {
        cout << "�������� ���������� �����.\n";
        return;
    }

    cout << "������� ����� �� 0 �� 7: ";
    for (int i = 0; i < count; ++i) {
        cin >> num;
        if (num >= 0 && num <= 7) {
            if ((bitArray >> num) & 1) {
                cout << "����� " << num << " ��� �������. ��������� ����.\n";
                --i;
            }
            else {
                bitArray |= (1 << num);
            }
        }
        else {
            cout << "�������� ����. ������� ����� �� 0 �� 7" << "\n";
            --i;
        }
    }

    cout << "��������������� �����: ";
    for (int i = 0; i < 8; ++i) {
        if ((bitArray >> i) & 1) {
            cout << i << " ";
        }
    }
    cout << "\n";
}

void Task_2b() {
    unsigned long long bitArray = 0;
    int num, count;

    cout << "������� ���������� ����� (�� 1 �� 64): ";
    cin >> count;

    if (count < 1 || count > 64) {
        cout << "�������� ���������� �����.\n";
        return;
    }

    cout << "������� ����� �� 0 �� 63: ";
    for (int i = 0; i < count; ++i) {
        cin >> num;
        if (num >= 0 && num <= 63) {
            if ((bitArray >> num) & 1) {
                cout << "����� " << num << " ��� �������. ��������� ����.\n";
                --i;
            }
            else {
                bitArray |= (1ULL << num);
            }
        }
        else {
            cout << "�������� ����. ������� ����� �� 0 �� 63" << "\n";
            --i;
        }
    }

    cout << "��������������� �����: ";
    for (int i = 0; i < 64; ++i) {
        if ((bitArray >> i) & 1) {
            cout << i << " ";
        }
    }
    cout << "\n";
}

void Task_2c() {
    vector<unsigned char> bitArray(8);
    int num, count;

    cout << "������� ���������� ����� (�� 1 �� 64): ";
    cin >> count;

    if (count < 1 || count > 64) {
        cout << "�������� ���������� �����.\n";
        return;
    }

    cout << "������� ����� �� 0 �� 63: ";
    for (int i = 0; i < count; ++i) {
        cin >> num;
        if (num >= 0 && num <= 63) {
            int byteIndex = num / 8;
            int bitIndex = num % 8;
            if ((bitArray[byteIndex] >> bitIndex) & 1) {
                cout << "����� " << num << " ��� �������. ��������� ����.\n";
                --i;
            }
            else {
                bitArray[byteIndex] |= (1 << bitIndex);
            }
        }
        else {
            cout << "�������� ����. ������� ����� �� 0 �� 63" << "\n";
            --i;
        }
    }

    cout << "��������������� �����: ";
    for (int i = 0; i < 64; ++i)
    {
        int byteIndex = i / 8;
        int bitIndex = i % 8;
        if ((bitArray[byteIndex] >> bitIndex) & 1)
        {
            cout << i << " ";
        }
    }
    cout << "\n";
}