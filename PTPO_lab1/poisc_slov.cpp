// poisc_slov.cpp
// Здесь пишу сами функции.

#include "poisc_slov.h"
#include <fstream>
#include <iostream>

// Убираю знаки препинания и делаю буквы маленькими
string slova_bez_zap(string slovo) {
    string result = "";

    // Прохожу по каждому символу и оставляю только буквы
    for (int i = 0; i < slovo.size(); i++) {
        char c = slovo[i];

        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
            c == 'ё' || c == 'Ё') {
            result = result + c;
        }
    }

    // Делаю всё маленькими буквами
    for (int i = 0; i < result.size(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + 32;
        }
        if (result[i] >= 'А' && result[i] <= 'Я') {
            result[i] = result[i] + 32;
        }
        if (result[i] == 'Ё') {
            result[i] = 'ё';
        }
    }

    return result;
}

// Считаю слова в файле
map<string, int> countWords(string filename) {
    map<string, int> slova;

    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Не могу открыть файл: " << filename << endl;
        return slova;
    }

    string slovo;
    while (file >> slovo) {
        string clean = slova_bez_zap(slovo);
        if (clean != "") {
            slova[clean] = slova[clean] + 1;
        }
    }

    file.close();
    return slova;
}

// Печатаю результат подсчёта
void printCount(map<string, int> slova) {
    cout << "Количество слов:" << endl;
    for (map<string, int>::iterator it = slova.begin(); it != slova.end(); it++) {
        cout << it->first << " - " << it->second << endl;
    }
}

// Ищу конкретное слово в файле
int findWord(string filename, string what) {
    string target = slova_bez_zap(what);

    if (target == "") {
        return 0;
    }

    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Не могу открыть файл: " << filename << endl;
        return 0;
    }

    int counter = 0;
    string slovo;

    while (file >> slovo) {
        string clean = slova_bez_zap(slovo);
        if (clean == target) {
            counter = counter + 1;
        }
    }

    file.close();
    return counter;
}

// Печатаю результат поиска (если понадобится)
void printFindResult(string what, int count) {
    cout << "Слово \"" << what << "\" встретилось " << count << " раз(а)" << endl;
}