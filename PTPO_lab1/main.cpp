// main.cpp
// Этап 2: подсчёт слов, позиции, поиск в цикле.
// + добавил вопрос: показывать ли полный вывод

#include "poisc_slov.h"
#include <iostream>
#include <ctime>
#include <direct.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

using namespace std;

int main() {
    // Настраиваю кодировку консоли для русского языка
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    cout << "Рабочая директория: " << cwd << endl;

    time_t startTime = time(NULL);

    string files[4] = {
        "tom1.txt",
        "tom2.txt",
        "tom3.txt",
        "tom4.txt"
    };

    // === Спрашиваю, показывать ли полный вывод ===
    cout << endl;
    cout << "Показывать полный список слов и позиций?" << endl;
    cout << "Это может занять несколько минут." << endl;
    cout << "Введите 'да' или 'нет': ";

    string otvet;
    cin >> otvet;

    bool pokazyvat = (otvet == "да" || otvet == "yes" ||
        otvet == "y" || otvet == "1");

    if (pokazyvat) {
        cout << endl;
        cout << "=== Подсчёт уникальных слов ===" << endl;

        for (int i = 0; i < 4; i++) {
            cout << endl;
            cout << "--- Файл: " << files[i] << " ---" << endl;

            map<string, int> words = countWords(files[i]);
            printCount(words);

            cout << endl;

            map<string, vector<int> > pozicii = getWordPositions(files[i]);
            printPositions(pozicii);
        }
    }
    else {
        cout << "Хорошо, пропускаю полный вывод." << endl;
    }

    time_t endTime = time(NULL);
    cout << endl;
    cout << "Общее время: " << (endTime - startTime) << " секунд" << endl;

    // === Поиск конкретного слова (в цикле) ===
        // === Поиск конкретного слова (в цикле) ===
    cout << endl;
    cout << "=== Поиск слова в файлах ===" << endl;
    cout << "Чтобы выйти, введите: выход" << endl;
    cout << endl;

    while (true) {
        cout << "Введите слово: ";

        string what;
        cin >> what;

        if (what == "выход" || what == "exit" || what == "quit") {
            cout << "Программа завершена. До свидания!" << endl;
            break;
        }

        int total = 0;

        // Прохожу по каждому файлу: ищу позиции и печатаю их
        for (int i = 0; i < 4; i++) {
            vector<int> nomera = findWordPositions(files[i], what);

            cout << "В файле " << files[i] << ": " << nomera.size() << " раз(а), ";
            printFoundPositions(nomera);
            cout << endl;

            total = total + nomera.size();
        }

        if (total == 0) {
            cout << "Слово \"" << what << "\" не найдено ни в одном файле." << endl;
        }

        cout << "Итого во всех файлах: " << total << " раз(а)" << endl;
        cout << endl;
    }

    return 0;
}