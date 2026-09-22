// poisc_slov.h
// Здесь объявляю все функции.
// Этап 1: подсчёт слов + поиск конкретного слова.
// Этап 2: добавил индексацию позиций слов.

#ifndef POISC_SLOV_H
#define POISC_SLOV_H

#include <string>
#include <map>
#include <vector>

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

// Запоминает, на каких позициях стоит каждое слово
map<string, vector<int> > getWordPositions(string filename);

// Печатает позиции слов в формате: слово – поз1, поз2, ...
void printPositions(map<string, vector<int> > pozicii);

// Ищет слово в файле и возвращает список его позиций
vector<int> findWordPositions(string filename, string what);

// Печатает позиции найденного слова
void printFoundPositions(vector<int> nomera);
#endif