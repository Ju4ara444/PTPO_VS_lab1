// main.cpp
// Этап 1: считаю слова в 4 файлах + ищу конкретное слово.

#include "poisc_slov.h"
#include <iostream>
#include <ctime>
#include <direct.h>  

// Подключаю windows.h для настройки кодировки консоли.
// ВАЖНО: до using namespace std, иначе будет ошибка "byte не является однозначным".
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

using namespace std;

int main() {
    // Настраиваю кодировку консоли для русского языка
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    // ДИАГНОСТИКА: показываю рабочую директорию, чтобы понимать,
    // откуда программа ищет файлы. Потом можно убрать.
    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    cout << "Рабочая директория: " << cwd << endl;

    cout << "=== Подсчёт уникальных слов ===" << endl;

    time_t startTime = time(NULL);

    // Файлы должны лежать РЯДОМ с main.cpp (или в рабочей директории)
    string files[4] = {
        "tom1.txt",
        "tom2.txt",
        "tom3.txt",
        "tom4.txt"
    };

    // Прохожу по каждому файлу: считаю слова и печатаю
    for (int i = 0; i < 4; i++) {
        cout << endl;
        cout << "--- Файл: " << files[i] << " ---" << endl;

        map<string, int> words = countWords(files[i]);
        printCount(words);
    }

    time_t endTime = time(NULL);
    cout << endl << "Общее время: " << (endTime - startTime) << " секунд" << endl;

    // === Поиск конкретного слова ===
    cout << endl;
    cout << "=== Поиск слова в файлах ===" << endl;
    cout << "Чтобы выйти, введите: выход" << endl;
    cout << endl;

    while (true) {
        cout << "Введите слово: ";

        string what;
        cin >> what;

        // Проверяю, не хочет ли пользователь выйти
        if (what == "выход" || what == "exit" || what == "quit") {
            cout << "Программа завершена. До свидания!" << endl;
            break;   // выходим из цикла while
        }

        // Ищу слово в каждом файле и суммирую
        int total = 0;
        for (int i = 0; i < 4; i++) {
            int found = findWord(files[i], what);
            cout << "В файле " << files[i] << ": " << found << " раз(а)" << endl;
            total = total + found;
        }

        cout << "Итого во всех файлах: " << total << " раз(а)" << endl;
        cout << endl;
    }

    return 0;
}