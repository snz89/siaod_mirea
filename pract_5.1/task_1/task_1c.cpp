#include <iostream>
#include <bitset>

using namespace std;

int main() {
	setlocale(0, "");
	unsigned int x = 25; // 00000000000000000000000000011001
	const int n = sizeof(int)*8; 
	cout << "Число x: " << bitset<n>(x) << endl;
	unsigned maska = (1 << n - 1); // 10000000000000000000000000000000
	cout << "Начальный вид маски: " << bitset<n>(maska) << endl;
	cout << "Результат: ";
	for (int i = 1; i <= n; i++) {
		cout << ((x & maska) >> (n - i));
		maska = maska >> 1;
	}
	cout << endl;
}

