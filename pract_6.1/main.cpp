#include "hash_table.h"
#include "book.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
    int menu = 0;
    unsigned long long inputIsbn = 0;
    string inputAuthor = "";
    string inputTitle = "";
    HashTable table = HashTable();

    while (true) {
        cout << string(90, '=') << "\n"
        << "1. Insert account\n"
        << "2. Remove account\n"
        << "3. Search account\n"
        << "4. Print table\n"
        << "5. Automatic table filling\n"
        << "6. Exit\n"
        << "---> ";
        cin >> menu;
        cout << string(90, '=') << "\n";

        switch (menu)
        {
        case 1:
            while (true) {
                cout << "Enter key: ";
                cin >> inputIsbn;
                if (inputIsbn >= 100000000000 && inputIsbn <= 999999999999) break;
                cout << "ISBN must be 12 digits" << "\n";
            }
            cout << "Enter name: ";
            cin >> inputAuthor;
            cout << "Enter address: ";
            cin >> inputTitle;
            if (table.insert(inputIsbn, inputAuthor, inputTitle))
                cout << "Book with ISBN " << inputIsbn << " added" << "\n";
            else
                cout << "Book with ISBN " << inputIsbn << " already exists" << "\n";
            break;
        case 2:
            cout << "Enter key: ";
            cin >> inputIsbn;
            if (table.remove(inputIsbn))
                cout << "Book with ISBN " << inputIsbn << " removed" << "\n";
            else
                cout << "Book with ISBN " << inputIsbn << " does not exist" << "\n";
            break;
        case 3:
            cout << "Enter key: ";
            cin >> inputIsbn;
            if (table.search(inputIsbn)) {
                Book* account = table.search(inputIsbn);
                cout << "Book with ISBN " << inputIsbn << " found ---> Author: "
                     << account->author << ", Title: " << account->title << "\n";
            }
            else
                cout << "Book with ISBN " << inputIsbn << " does not exist" << "\n";
            break;
        case 4:
            table.print();
            break;
        case 5:
            table.insert(9780140449136, "Homer", "The Odyssey");
            table.insert(9780451524935, "George Orwell", "1984");
            table.insert(9780061120084, "Harper Lee", "To Kill a Mockingbird");
            table.insert(9780439023528, "Suzanne Collins", "The Hunger Games");
            table.insert(9780743273565, "F. Scott Fitzgerald", "The Great Gatsby");
            table.insert(9780316769488, "J.D. Salinger", "The Catcher in the Rye");
            table.insert(9780143039433, "Mark Twain", "Adventures of Huckleberry Finn");
            cout << "Table filled with 7 accounts" << "\n";
            break;
        case 6:
            return 0;
        default:
            break;
        }
    }
}

