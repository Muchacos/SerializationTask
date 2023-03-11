#include <iostream>
#include <cstdio>

#include "ListRand.h"

// Возвращает Id-элемент списка. Не совершает проверок передаваемых данных. Используется для дебага.
ListNode* UnsafeGetNode(ListRand* List, int Id, ListNode* Node = nullptr)
{
    if (Node == nullptr)
    {
        Node = List->Head;
    }
    
    if (Id == 0)
    {
        return Node;
    }
    
    return UnsafeGetNode(List, Id - 1, Node->Next);
}

// Выводит содержимое списка. Не совершает проверок передаваемых данных. Используется для дебага.
void UnsafePrintList(ListRand* List)
{
    std::cout << List->Count << "\n";

    for (int i = 0; i < List->Count; ++i)
    {
        ListNode* Node = UnsafeGetNode(List, i);
        std::cout << "Data: " << Node->Data << " RandData: " << (Node->Rand ? Node->Rand->Data : "nullptr") << "\n";
    }
}

int main(int argc, char* argv[])
{
    ListRand List;
    List.Add("");
    List.Add("String 1");
    List.Add("String 2__");
    List.Add("String 3");
    List.Add("String 4");

    UnsafeGetNode(&List, 1)->Rand = UnsafeGetNode(&List, 1);
    UnsafeGetNode(&List, 2)->Rand = UnsafeGetNode(&List, 4);
    UnsafeGetNode(&List, 3)->Rand = List.Head;

    // Записываем список в файл
    FILE* Stream;
    fopen_s(&Stream, "data.bin", "wb");
    List.Serialize(Stream);
    fclose(Stream);

    UnsafePrintList(&List);

    // Создаем новый список, используя данные из файла
    ListRand LoadedList;
    fopen_s(&Stream, "data.bin", "rb");
    LoadedList.Deserialize(Stream);
    fclose(Stream);

    UnsafePrintList(&LoadedList);
    
    return 0;
}
