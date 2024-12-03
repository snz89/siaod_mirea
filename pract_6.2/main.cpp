#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

/**
 * Разделяет текст на слова и фильтрует слова, начинающиеся с заданной подстроки.
 * @param text Текст для разбиения на слова.
 * @param substring Заданная подстрока для фильтрации.
 * @return Вектор слов, начинающихся с заданной подстроки.
 */
vector<string> splitAndFilter(const string& text, const string& substring) {
    string punctuation = ",.!?;:-";
    string cleanedText = text;

    for (char& ch : cleanedText) {
        if (punctuation.find(ch) != string::npos) {
            ch = ' ';
        }
    }

    vector<string> words;
    istringstream stream(cleanedText);
    string word;
    while (stream >> word) {
        if (word.size() >= substring.size() && 
            equal(substring.begin(), substring.end(), word.begin(), 
                       [](char a, char b) { return tolower(a) == tolower(b); })) {
            words.push_back(word);
        }
    }

    return words;
}

/**
 * Дан текст из слов, разделенных знаками препинания.
 * Сформировать массив из слов, в которых заданная подстрока размещается с первой позиции
 * @param fileName Имя файла для чтения текста.
 * @param substring Заданная подстрока для фильтрации.
 */
void task_1(const string& fileName, const string& substring) {
    ifstream file(fileName);

    if (!file) {
        cerr << "Error: can't open file: " << fileName << endl;
        return;
    }

    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    file.close();

    vector<string> filteredWords = splitAndFilter(text, substring);

    cout << "Source text: " << text << "\n"
    << "Prefix pattern: " << substring << "\n";
    cout << "Words, starts with \"" << substring << "\": ";
    for (const auto& word : filteredWords) {
        cout << word << " ";
    }
}

// Функция для построения таблицы смещений по плохому символу
vector<int> badCharTable(const string& pattern) {
    int patternLength = pattern.size();
    vector<int> badChar(256, -1);
    
    for (int i = 0; i < patternLength; ++i) {
        badChar[static_cast<unsigned char>(pattern[i])] = i;
    }
    return badChar;
}


// Функция для построения таблицы смещений по хорошему суффиксу
vector<int> goodSuffixTable(const string& pattern) {
    int patternLength = pattern.size();
    vector<int> goodSuffix(patternLength + 1, patternLength);

    vector<int> border(patternLength + 1, 0);
    int patternIndex = patternLength;
    int borderIndex = patternLength + 1;

    border[patternIndex] = borderIndex;
    while (patternIndex > 0) {
        while (borderIndex <= patternLength && pattern[patternIndex - 1] != pattern[borderIndex - 1]) {
            if (goodSuffix[borderIndex] == patternLength) {
                goodSuffix[borderIndex] = borderIndex - patternIndex;
            }
            borderIndex = border[borderIndex];
        }
        --patternIndex;
        --borderIndex;
        border[patternIndex] = borderIndex;
    }

    for (int i = 0; i <= patternLength; ++i) {
        if (goodSuffix[i] == patternLength) {
            goodSuffix[i] = borderIndex;
        }
        if (i == borderIndex) {
            borderIndex = border[borderIndex];
        }
    }

    return goodSuffix;
}

vector<int> TurboBoyerMooreSearch(const string& text, const string& pattern) {
    int patternLength = pattern.length();
    int textLength = text.length();
    int textIndex = 0;
    int turboShiftLength = 0;
    int shift = patternLength;
    vector<int> matchPositions;

    if (patternLength == 0) {
        return matchPositions;
    }

    vector<int> badCharacterShift = badCharTable(pattern);
    vector<int> goodSuffixShift = goodSuffixTable(pattern);

    while (textIndex <= textLength - patternLength) {
        int patternIndex = patternLength - 1;
        while (patternIndex >= 0 && pattern[patternIndex] == text[textIndex + patternIndex]) {
            --patternIndex;
            if (turboShiftLength != 0 && patternIndex == patternLength - 1 - shift) {
                patternIndex -= turboShiftLength;
            }
        }
        if (patternIndex < 0) {
            matchPositions.push_back(textIndex);
            shift = goodSuffixShift[0];
            turboShiftLength = patternLength - shift;
        } else {
            int v = patternLength - 1 - patternIndex;
            int turboShift = turboShiftLength - v;
            int bCShift = badCharacterShift[text[textIndex + patternIndex]] - patternLength + patternIndex + 1;
            shift = max({turboShift, bCShift, goodSuffixShift[patternIndex + 1]});
            if (shift == goodSuffixShift[patternIndex + 1]) {
                turboShiftLength = min((patternLength - shift), v);
            } else {
                if (turboShift < bCShift) {
                    shift = min(shift, (turboShiftLength + 1));
                }
                turboShiftLength = 0;
            }
        }
        textIndex += shift;
    }
    return matchPositions;
}

void task_2(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file!" << endl;
        return;
    }

    string pattern, text;
    getline(file, pattern);
    getline(file, text);

    file.close();

    if (pattern.empty() || text.empty()) {
        cerr << "Error: Pattern or text is empty!" << endl;
        return;
    }

    cout << "Source text: " << text << "\n"
    << "Pattern: " << pattern << "\n"
    << "Matches: ";

    auto v = TurboBoyerMooreSearch(text, pattern);
    for (auto i : v) {
        cout << i << " ";
    }
    cout << endl;
}

int main() {
    task_2("task_2.txt");
}
