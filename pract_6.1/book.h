#ifndef BOOK
#define BOOK

#include <string>
using namespace std;

struct Book
{
    unsigned long long isbn;
    string author;
    string title;
    bool deleted;
};

#endif
