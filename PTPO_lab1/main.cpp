// main.cpp
// ============================================================
// Точка входа программы. Здесь я:
//   1) подключаю всё нужное,
//   2) настраиваю русский язык в консоли,
//   3) вызываю функции для файлов и вектора.
// ============================================================

#include "poisc_slov.h"    // Мои функции.
#include <iostream>        // Для cout и cin.
#include <ctime>           // Для time() - замер времени.
#include <direct.h>        // Для _getcwd() - диагностика рабочей директории.

// Для настройки кодировки консоли на CP1251.
// ВАЖНО: подключать ДО using namespace std, иначе будет ошибка
// "byte не является однозначным" (конфликт Windows byte и std::byte).
#define WIN32_LEAN_AND_MEAN   // Отключает редко используемые части Windows - компиляция быстрее.
#define NOMINMAX              // Отключает макросы min/max, чтобы не мешали std::min/std::max.
#include <windows.h>

using namespace std;

int main() {
    // Настраиваю кодировку консоли, чтобы русские буквы отображались нормально.
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    // Диагностика: показываю, откуда программа ищет файлы.
    // Это помогло мне понять, что файлы должны лежать рядом с .vcxproj.
    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    cout << "Рабочая директория: " << cwd << endl;

    // Засекаю время: замеряю, сколько заняла вся обработка.
    time_t startTime = time(NULL);

    // Четыре тома "Войны и мира". Файлы лежат рядом с main.cpp.
    string files[4] = {
        "tom1.txt",
        "tom2.txt",
        "tom3.txt",
        "tom4.txt"
    };

    // ---- Спрашиваю, показывать ли полный вывод ----
    // Полный вывод (все слова + все позиции) может занять минуты.
    // Если пользователю нужно только искать слова - экономим время.
    cout << endl;
    cout << "Показывать полный список слов и позиций?" << endl;
    cout << "Это может занять несколько минут." << endl;
    cout << "Введите 'да' или 'нет': ";

    string otvet;
    cin >> otvet;

    // Считаю ответ "да" разными способами, чтобы было удобно.
    bool pokazyvat = (otvet == "да" || otvet == "yes" ||
        otvet == "y" || otvet == "1");

    if (pokazyvat) {
        cout << endl;
        cout << "=== Подсчёт уникальных слов ===" << endl;

        // Прохожу по каждому из 4 файлов.
        for (int i = 0; i < 4; i++) {
            cout << endl;
            cout << "--- Файл: " << files[i] << " ---" << endl;

            // Этап 1: подсчёт слов в файле.
            map<string, int> words = countWords(files[i]);
            printCount(words);

            cout << endl;

            // Этап 2: позиции всех слов в файле.
            map<string, vector<int> > pozicii = getWordPositions(files[i]);
            printPositions(pozicii);
        }
    }
    else {
        cout << "Хорошо, пропускаю полный вывод." << endl;
    }

    // Останавливаю таймер и печатаю результат.
    time_t endTime = time(NULL);
    cout << endl;
    cout << "Общее время: " << (endTime - startTime) << " секунд" << endl;

    // ---- Цикл поиска конкретных слов ----
    // Работает, пока пользователь не введёт "выход".
    cout << endl;
    cout << "=== Поиск слова в файлах ===" << endl;
    cout << "Чтобы выйти, введите: выход" << endl;
    cout << endl;

    while (true) {
        cout << "Введите слово: ";

        string what;
        cin >> what;   // Читаю слово, которое ищем.

        // Проверяю команды выхода.
        if (what == "выход" || what == "exit" || what == "quit") {
            cout << "Программа завершена. Переход к этапу 3." << endl;
            break;   // Выхожу из цикла while.
        }

        int total = 0;   // Общий счётчик по всем файлам.

        // Ищу слово в каждом из 4 файлов.
        for (int i = 0; i < 4; i++) {
            // nomera - список позиций, где встретилось слово.
            vector<int> nomera = findWordPositions(files[i], what);

            // nomera.size() - сколько раз встретилось.
            cout << "В файле " << files[i] << ": " << nomera.size() << " раз(а), ";
            printFoundPositions(nomera);   // Печатаю сами позиции.
            cout << endl;

            total = total + nomera.size();
        }

        // Если нигде не нашли - сообщаю отдельно.
        if (total == 0) {
            cout << "Слово \"" << what << "\" не найдено ни в одном файле." << endl;
        }

        cout << "Итого во всех файлах: " << total << " раз(а)" << endl;
        cout << endl;
    }

    // ============================================================
    // ЭТАП 3: работа с vector<int>
    // ============================================================

    cout << endl;
    cout << "=== Этап 3. Работа с vector<int> ===" << endl;

    // Создаю вектор с числами (push_back добавляет в конец).
    vector<int> chisla;
    chisla.push_back(10);
    chisla.push_back(7);
    chisla.push_back(14);
    chisla.push_back(3);
    chisla.push_back(5);
    chisla.push_back(9);
    chisla.push_back(2);
    chisla.push_back(8);
    chisla.push_back(11);
    chisla.push_back(6);
    chisla.push_back(4);
    chisla.push_back(1);
    chisla.push_back(13);

    // Печатаю исходный вектор.
    cout << endl;
    cout << "Исходный вектор: ";
    pechatVector(chisla);

    // Пункт 3а: простые числа -> их квадраты.
    kvadratProstyh(chisla);
    cout << "После возведения простых в квадрат: ";
    pechatVector(chisla);

    // Пункт 3б: нечётные по возрастанию, потом чётные по убыванию.
    sortNechetChet(chisla);
    cout << "После сортировки (нечётные ↑, чётные ↓): ";
    pechatVector(chisla);

    // Пункт 3в: уникальные числа в диапазоне [5, 15].
    int low = 5;
    int high = 15;
    vector<int> naidennye = naydiVDiapazone(chisla, low, high);

    cout << "Уникальные числа в диапазоне [" << low << ", " << high << "]: ";
    pechatVector(naidennye);

    return 0;   // Программа успешно завершилась.
}