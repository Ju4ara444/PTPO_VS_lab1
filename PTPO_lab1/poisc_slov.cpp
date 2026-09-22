// poisc_slov.cpp
// ============================================================
// Здесь я ПИШУ сами функции, объявленные в poisc_slov.h.
// Комментарии - для себя и для защиты: объясняю, что делает
// каждая строчка и почему именно так.
// ============================================================

#include "poisc_slov.h"    // Подключаю свой заголовок - там объявления.
#include <fstream>         // Для ifstream - чтения файла.
#include <iostream>        // Для cout - вывода на экран.
#include <algorithm>       // Для sort() и reverse() из STL.

// ============================================================
// ЭТАП 1: слова и файлы
// ============================================================

// Убираю знаки препинания и привожу слово к нижнему регистру.
// Зачем: чтобы "Свобода" и "свобода" считались одним словом.
string slova_bez_zap(string slovo) {
    string result = "";   // Сюда буду собирать "чистое" слово.

    // Иду по каждому символу исходного слова.
    // Оставляю только буквы: a-z, A-Z (латиница) и а-я, А-Я, ё, Ё (кириллица).
    for (int i = 0; i < slovo.size(); i++) {
        char c = slovo[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
            c == 'ё' || c == 'Ё') {
            result = result + c;
        }
    }

    // Теперь делаю все буквы строчными (маленькими).
    // В таблице ASCII/CP1251 большие и маленькие буквы отличаются
    // ровно на 32. Поэтому прибавляю 32 к большим буквам.
    for (int i = 0; i < result.size(); i++) {
        // Латиница: A-Z (65-90) -> a-z (97-122)
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + 32;
        }
        // Кириллица: А-Я идут подряд, сдвиг тоже 32
        if (result[i] >= 'А' && result[i] <= 'Я') {
            result[i] = result[i] + 32;
        }
        // Буква Ё стоит отдельно в таблице, поэтому с ней отдельно
        if (result[i] == 'Ё') {
            result[i] = 'ё';
        }
    }

    return result;
}

// Считаю слова в файле.
// map - контейнер "ключ-значение", где ключи автоматически сортируются.
// Если slova["анна"] ещё нет - он создастся со значением 0.
// Если есть - вернётся текущее значение, к нему прибавим 1.
map<string, int> countWords(string filename) {
    map<string, int> slova;   // Здесь будем хранить: слово -> количество.

    // Открываю файл. filename.c_str() - потому что ifstream хочет C-строку.
    ifstream file(filename.c_str());

    // Проверяю, открылся ли файл. Если нет - сообщаю и возвращаю пустой map.
    if (!file.is_open()) {
        cout << "Не могу открыть файл: " << filename << endl;
        return slova;
    }

    // file >> slovo читает ОДНО слово за раз.
    // Пробелы и переводы строк автоматически пропускаются.
    string slovo;
    while (file >> slovo) {
        string clean = slova_bez_zap(slovo);
        // Если после очистки что-то осталось (не одна пунктуация) - считаю.
        if (clean != "") {
            slova[clean] = slova[clean] + 1;
        }
    }

    file.close();   // Хорошая практика - закрывать файл, когда больше не нужен.
    return slova;
}

// Печатаю map со словами. Он уже отсортирован по алфавиту автоматически.
void printCount(map<string, int> slova) {
    cout << "Количество слов:" << endl;
    // Итератор it "ходит" по всем парам (ключ, значение) в map.
    for (map<string, int>::iterator it = slova.begin(); it != slova.end(); it++) {
        // it->first  - это ключ (слово)
        // it->second - это значение (количество)
        cout << it->first << " - " << it->second << endl;
    }
}

// Ищу одно слово в файле, возвращаю количество вхождений.
// Логика похожа на countWords, но не запоминаю все слова, а только ищу нужное.
int findWord(string filename, string what) {
    string target = slova_bez_zap(what);   // Привожу искомое слово к тому же виду.
    if (target == "") return 0;            // Пустое искать бессмысленно.

    ifstream file(filename.c_str());
    if (!file.is_open()) return 0;

    int counter = 0;   // Счётчик вхождений.
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

// Просто печатаю результат поиска.
void printFindResult(string what, int count) {
    cout << "Слово \"" << what << "\" встретилось " << count << " раз(а)" << endl;
}

// ============================================================
// ЭТАП 2: позиции слов
// ============================================================

// Запоминаю, на каких позициях стоит каждое слово.
// Позиция - это номер слова в файле, начиная с 0.
// Пример: "Свобода творчества — свобода делать ошибки"
//   свобода    -> [0, 2]
//   творчества -> [1]
//   делать     -> [3]
//   ошибки     -> [4]
map<string, vector<int> > getWordPositions(string filename) {
    map<string, vector<int> > pozicii;   // слово -> список позиций

    ifstream file(filename.c_str());
    if (!file.is_open()) return pozicii;

    string slovo;
    int nomer = 0;   // Номер текущего слова. Начинаю с нуля.

    while (file >> slovo) {
        string clean = slova_bez_zap(slovo);
        if (clean != "") {
            // pozicii[clean] - это vector<int>. push_back() добавляет
            // новый номер в конец списка.
            pozicii[clean].push_back(nomer);
        }
        nomer = nomer + 1;   // Следующее слово получит следующий номер.
    }

    file.close();
    return pozicii;
}

// Печатаю позиции всех слов.
void printPositions(map<string, vector<int> > pozicii) {
    cout << "Позиции слов:" << endl;

    for (map<string, vector<int> >::iterator it = pozicii.begin();
        it != pozicii.end(); it++) {

        cout << it->first << " – ";   // Печатаю слово.

        vector<int> nomera = it->second;   // Список позиций этого слова.
        for (int i = 0; i < nomera.size(); i++) {
            cout << nomera[i];
            // Запятую ставлю после всех номеров, кроме последнего.
            if (i != nomera.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// Ищу слово и возвращаю ВСЕ его позиции (вектор номеров).
vector<int> findWordPositions(string filename, string what) {
    vector<int> nomera;
    string target = slova_bez_zap(what);
    if (target == "") return nomera;

    ifstream file(filename.c_str());
    if (!file.is_open()) return nomera;

    string slovo;
    int poziciya = 0;

    while (file >> slovo) {
        string clean = slova_bez_zap(slovo);
        if (clean == target) {
            nomera.push_back(poziciya);   // Добавляю позицию в вектор.
        }
        poziciya = poziciya + 1;
    }

    file.close();
    return nomera;
}

// Печатаю позиции. Если их больше 10 - показываю только первые 10.
// Иначе консоль "поплывёт" от тысяч чисел для частых слов ("и", "в").
void printFoundPositions(vector<int> nomera) {
    if (nomera.size() == 0) {
        cout << "позиций нет";
        return;
    }

    cout << "позиции: ";

    // Определяю, сколько печатать: 10 или меньше (если их меньше).
    int limit = nomera.size();
    if (limit > 10) {
        limit = 10;
    }

    for (int i = 0; i < limit; i++) {
        cout << nomera[i];
        if (i != limit - 1) {
            cout << ", ";
        }
    }

    // Если позиций было больше 10 - пишу "... (всего N)".
    if (nomera.size() > 10) {
        cout << ", ... (всего " << nomera.size() << ")";
    }
}

// ============================================================
// ЭТАП 3: работа с vector<int>
// ============================================================

// Проверяю, простое ли число.
// Простое - натуральное число > 1, которое делится только на 1 и на себя.
// Например: 2, 3, 5, 7, 11, 13, 17, 19, 23, ...
// Не простые: 0, 1, 4, 6, 8, 9, 10, 12, ...
bool isProstoe(int chislo) {
    if (chislo < 2) {
        return false;   // 0 и 1 не простые по определению.
    }

    // Пробую делить на все числа от 2 до chislo - 1.
    // Если хоть на что-то делится без остатка - значит не простое.
    for (int d = 2; d < chislo; d++) {
        if (chislo % d == 0) {
            return false;
        }
    }
    return true;   // Ни на что не делилось - простое.
}

// Пункт 3а: заменяю простые числа на их квадраты.
// Вектор передаю ПО ССЫЛКЕ (&), чтобы изменения применились к оригиналу.
void kvadratProstyh(vector<int>& chisla) {
    for (int i = 0; i < chisla.size(); i++) {
        if (isProstoe(chisla[i])) {
            chisla[i] = chisla[i] * chisla[i];   // Возвожу в квадрат.
        }
    }
}

// Пункт 3б: сортировка "нечётные ↑, потом чётные ↓".
// Идея простая: разделяю вектор на два (нечётные и чётные),
// сортирую каждый по-своему, а потом склеиваю обратно.
void sortNechetChet(vector<int>& chisla) {
    vector<int> nechetnye;   // Сюда сложу нечётные.
    vector<int> chetnye;     // Сюда - чётные.

    // Шаг 1: раскладываю числа по двум векторам.
    // x % 2 != 0 - значит число нечётное (остаток от деления на 2 не 0).
    for (int i = 0; i < chisla.size(); i++) {
        if (chisla[i] % 2 != 0) {
            nechetnye.push_back(chisla[i]);
        }
        else {
            chetnye.push_back(chisla[i]);
        }
    }

    // Шаг 2: сортирую нечётные по возрастанию.
    // std::sort по умолчанию сортирует по возрастанию.
    sort(nechetnye.begin(), nechetnye.end());

    // Шаг 3: сортирую чётные по возрастанию, потом переворачиваю -
    // так получаю порядок по убыванию.
    sort(chetnye.begin(), chetnye.end());
    reverse(chetnye.begin(), chetnye.end());

    // Шаг 4: собираю обратно в исходный вектор.
    chisla.clear();   // Очищаю всё, что было.

    // Сначала нечётные (они уже по возрастанию).
    for (int i = 0; i < nechetnye.size(); i++) {
        chisla.push_back(nechetnye[i]);
    }

    // Затем чётные (они уже по убыванию).
    for (int i = 0; i < chetnye.size(); i++) {
        chisla.push_back(chetnye[i]);
    }
}

// Пункт 3в: ищу уникальные числа в диапазоне [low, high].
// Вход: вектор чисел и границы диапазона.
// Выход: отсортированный вектор из уникальных чисел, попадающих в диапазон.
vector<int> naydiVDiapazone(vector<int> chisla, int low, int high) {
    vector<int> result;   // Здесь буду накапливать уникальные числа.

    for (int i = 0; i < chisla.size(); i++) {
        int x = chisla[i];

        // Проверяю, попадает ли число в диапазон [low, high].
        if (x >= low && x <= high) {

            // Проверяю, нет ли уже такого числа в result.
            // Иду по result циклом и сравниваю каждое значение с x.
            bool uzheEst = false;
            for (int j = 0; j < result.size(); j++) {
                if (result[j] == x) {
                    uzheEst = true;
                    break;   // Нашли дубликат - дальше искать смысла нет.
                }
            }

            // Если такого числа ещё не было - добавляю.
            if (uzheEst == false) {
                result.push_back(x);
            }
        }
    }

    // Сортирую по возрастанию, чтобы вывод был красивым.
    sort(result.begin(), result.end());
    return result;
}

// Печатаю вектор в одну строку через пробел.
void pechatVector(vector<int> chisla) {
    for (int i = 0; i < chisla.size(); i++) {
        cout << chisla[i] << " ";
    }
    cout << endl;
}