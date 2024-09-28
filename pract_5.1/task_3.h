#ifndef TASK_3_H
#define TASK_3_H

#include <string>
#include <vector>

using namespace std;

void generateUniqueNumbersFile(const string& filename, size_t count);
void fillBitArray(const string& filename, vector<unsigned char>& bitArray);
void generateOutputFile(const string& filename, vector<unsigned char> bitArray);
void Task_3(size_t numberCount);

#endif