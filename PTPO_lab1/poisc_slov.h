// poisc_slov.h
// Здесь объявляю все функции для первого задания.
// Разделил на .h и .cpp, как просил преподаватель.

#ifndef POISC_SLOV_H
#define POISC_SLOV_H

#include <string>
#include <map>

using namespace std;

// Убирает знаки препинания с краёв слова и делает буквы маленькими
string slova_bez_zap(string slovo);

// Считает, сколько раз каждое слово встречается в файле
map<string, int> countWords(string filename);

// Печатает результат: слово - количество
void printCount(map<string, int> slova);

// Ищет конкретное слово в файле и возвращает, сколько раз оно встретилось
int findWord(string filename, string what);

// Печатает, сколько раз встретилось искомое слово
void printFindResult(string what, int count);

#endif