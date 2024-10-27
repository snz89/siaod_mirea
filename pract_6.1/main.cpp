#include "hash_table.h"
#include "bank_account.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
    int menu = 0;
    int inputKey = 0;
    string inputName = "";
    string inputAddress = "";
    HashTable table = HashTable();

    while (true) {
        cout << "==============================\n"
        << "1. Insert account\n"
        << "2. Remove account\n"
        << "3. Search account\n"
        << "4. Print table\n"
        << "5. Automatic table filling\n"
        << "6. Exit\n"
        << "---> ";
        cin >> menu;
        cout << "==============================\n";

        switch (menu)
        {
        case 1:
            while (true) {
                cout << "Enter key: ";
                cin >> inputKey;
                if (inputKey >= 1000000 && inputKey <= 9999999) break;
                cout << "Key must be 7 digits" << "\n";
            }
            cout << "Enter name: ";
            cin >> inputName;
            cout << "Enter address: ";
            cin >> inputAddress;
            if (table.insert(inputKey, inputName, inputAddress))
                cout << "Bank account with ID " << inputKey << " added" << "\n";
            else
                cout << "Bank account with ID " << inputKey << " already exists" << "\n";
            break;
        case 2:
            cout << "Enter key: ";
            cin >> inputKey;
            if (table.remove(inputKey))
                cout << "Bank account with ID " << inputKey << " removed" << "\n";
            else
                cout << "Bank account with ID " << inputKey << " does not exist" << "\n";
            break;
        case 3:
            cout << "Enter key: ";
            cin >> inputKey;
            if (table.search(inputKey)) {
                BankAccount* account = table.search(inputKey);
                cout << "Bank account with ID " << inputKey << " found ---> Name: "
                     << account->fullName << ", Address: " << account->address << "\n";
            }
            else
                cout << "Bank account with ID " << inputKey << " does not exist" << "\n";
            break;
        case 4:
            table.print();
            break;
        case 5:
            table.insert(1234567, "John Doe", "123 Main St");
            table.insert(2345678, "Jane Smith", "456 Elm St");
            table.insert(3456789, "Bob Johnson", "789 Oak St");
            table.insert(4567890, "Alice Brown", "101 Pine St");
            table.insert(5678901, "Charlie Davis", "202 Maple St");
            table.insert(6789012, "David Wilson", "303 Cedar St");
            table.insert(7890123, "Eva Martinez", "404 Birch St");
            cout << "Table filled with 7 accounts" << "\n";
            break;
        case 6:
            return 0;
        default:
            break;
        }
    }
}

