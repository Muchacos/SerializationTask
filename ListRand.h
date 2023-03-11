#pragma once

#include <string>
#include <unordered_map>

class ListNode
{
public:
    ListNode* Prev;
    ListNode* Next;
    ListNode* Rand;
    std::string Data;

    ListNode(const std::string& Data);
};

class ListRand
{
public:
    ListNode* Head;
    ListNode* Tail;
    int Count; // Желательно использовать тип с фиксированным размером для сериализации

    ListRand();
    ~ListRand();

    /** Добавляет элемент в список, создавая связи */
    void Add(const std::string& Data);
    /** Удаляет все элементы списка */
    void Clear();

    /** Приводит список к двоичному виду и записывает полученные данные в переданный файл */
    void Serialize(FILE* Stream) const;
    /** Заменяет все текущие данные списка на те, что записаны в двоичном файле */
    void Deserialize(FILE* Stream);

private:
    /** Записывает все элементы списка в хеш-таблицу с их индексами */
    void ToHashmap(std::unordered_map<ListNode*, int>& OutHashmap) const;
};
