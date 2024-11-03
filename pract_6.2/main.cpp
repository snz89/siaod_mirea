#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Функция для чтения файла в строку
string fileToString(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Функция для разделения строки на слова
vector<string> splitIntoWords(const string& text) {
    vector<string> words;
    string currentWord;

    for (char c : text) {
        if (isalnum(c)) {
            currentWord += c;
        } else if (!currentWord.empty()) {
            words.push_back(currentWord);
            currentWord = "";
        }
    }
    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }

    return words;
}

// Функция для проверки окончания слова на заданную подстроку
bool endsWithSubstring(const string& word, const string& substring) {
    if (word.length() < substring.length()) {
        return false;
    }
    return word.substr(word.length() - substring.length()) == substring;
}

// Функция для формирования массива слов, оканчивающихся заданной подстрокой
vector<string> filterWordsEndingWithSubstring(const vector<string>& words, const string& substring) {
    vector<string> filteredWords;
    for (const string& word : words) {
        if (endsWithSubstring(word, substring)) {
            filteredWords.push_back(word);
        }
    }
    return filteredWords;
}

// Дан текст, состоящий из слов, разделенных знаками препинания. Сформировать
// массив из слов, в которых заданная подстрока размещается в конце слова
void Task1(const string& inputFile, const string& substring) {
    auto text = fileToString(inputFile);
    auto words = splitIntoWords(text);
    auto filteredWords = filterWordsEndingWithSubstring(words, substring);
    cout << "[TASK 1] Words ending with '" << substring << "': ";
    for (const string& word : filteredWords) {
        cout << word << " ";
    }
    cout << "\n";
}

// Функция для чтения данных из файла
pair<string, string> readPairDataFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return {"", ""}; // Возвращаем пустые строки в случае ошибки
    }
    string pattern, text;
    getline(file, pattern);
    getline(file, text);
    file.close();
    return {pattern, text};
}

// Функция для вычисления хеша строки
long long hashString(const string& s, int prime, int base) {
    long long hash = 0;
    long long power = 1;
    for (int i = 0; i < s.length(); ++i) {
        hash = (hash + (s[i] * power)) % prime;
        power = (power * base) % prime;
    }
    return hash;
}

// Функция для реализации алгоритма Рабина-Карпа
int rabinKarp(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();
    if (m > n) {
        return 0;
    }

    int prime = 101; // Или большее простое число, например, 1000000007
    int base = 256;   

    long long patternHash = hashString(pattern, prime, base);
    long long textHash = hashString(text.substr(0, m), prime, base);
    long long power = 1;
    for (int i = 0; i < m - 1; ++i) {
        power = (power * base) % prime;
    }

    int count = 0;
    for (int i = 0; i <= n - m; ++i) {
        if (patternHash == textHash) {
            if (text.substr(i, m) == pattern) {
                count++;
            }
        }
        if (i < n - m) {
            textHash = (1LL * base * (textHash - 1LL * text[i] * power) + 1LL * text[i + m]) % prime;
            if (textHash < 0) {
                textHash += prime;
            }
        }
    }
    return count;
}


/*
 * В текстовом файле хранятся входные данные:
 * на первой сроке – подстрока (образец)длиной не более 17 символов для поиска в тексте;
 * со второй строки – текст (строка), в котором осуществляется поиск образца.
 * Строка, в которой надо искать, не ограниченна по длине.
 * Применяя алгоритм Рабина-Карпа определить количество вхождений в текст заданного образца
*/
int Task2(const string& inputFile) {
    pair<string, string> data = readPairDataFromFile(inputFile);

    if (data.first.empty() || data.second.empty()) {
        return 1;
    }

    int occurrences = rabinKarp(data.second, data.first);
    cout << "[TASK 2] Occurrences: " << occurrences << endl;

    return 0;
}

int main() {
    Task1("example_1.txt", "ing");
    Task2("example_2.txt");
}