#ifndef BANK_ACCOUNT
#define BANK_ACCOUNT

#include <string>
using namespace std;

struct BankAccount
{
    int id;
    string fullName;
    string address;
    bool deleted;
};

#endif
