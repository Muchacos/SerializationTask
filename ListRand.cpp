#include "ListRand.h"

ListNode::ListNode(const std::string& Data)
{
    this->Prev = nullptr;
    this->Next = nullptr;
    this->Rand = nullptr;
    this->Data = Data;
}

ListRand::ListRand()
{
    Head = nullptr;
    Tail = nullptr;
    Count = 0;
}

ListRand::~ListRand()
{
    Clear();
}

void ListRand::Add(const std::string& Data)
{
    ListNode* NewNode = new ListNode(Data);
    if (!Head)
    {
        Head = NewNode;
    }
    else
    {
        Tail->Next = NewNode;
        NewNode->Prev = Tail;
    }
    Tail = NewNode;
    ++Count;
}

void ListRand::Clear()
{
    if (!Head)
    {
        return;
    }

    ListNode* Node = Head;
    while (Node)
    {
        ListNode* Next = Node->Next;
        delete Node;
        Node = Next;
    }

    Head = nullptr;
    Tail = nullptr;
    Count = 0;
}

void ListRand::Serialize(FILE* Stream) const
{
    // Запись кол-ва элементов списка. Можно обойтись и без этой записи, если в итоговом бинарном файле не будет
    // записано ничего, кроме данных списка.
    fwrite(&Count, sizeof(Count), 1, Stream);
    
    std::unordered_map<ListNode*, int> Hashmap;
    ToHashmap(Hashmap);

    const ListNode* Node = Head;
    while (Node)
    {
        // Запись строки Data
        size_t StrLen = Node->Data.length();
        fwrite(&StrLen, sizeof(StrLen), 1, Stream); // Ее размер
        fwrite(&Node->Data[0], sizeof(char), StrLen, Stream); // Сама строка

        // Запись индексов rand-элементов. Записываем -1, если он не присвоен
        if (!Node->Rand)
        {
            constexpr int MinusOne = -1; // создаем переменную, т.к. fwrite хочет адрес в памяти, который rvalue не имеет
            fwrite(&MinusOne, sizeof(Count), 1, Stream);
        }
        else
        {
            fwrite(&Hashmap.at(Node->Rand), sizeof(Count), 1, Stream);
        }
        Node = Node->Next;
    }
}

void ListRand::Deserialize(FILE* Stream)
{
    Clear();

    int DeserializedCount;
    fread(&DeserializedCount, sizeof(Count), 1, Stream);

    std::vector<ListNode*> Nodes;

    // Создаем элементы со связями
    for (int i = 0; i < DeserializedCount; ++i)
    {
        
        Add("");  // Можно сразу проходить по файлу, заполняя строки Data у элементов при создании. Но тогда
                        // Но тогда будет неудобно считывать индексы rand-элементов, либо нужно будет записывать строки
                        // и индексы по очереди.
        Nodes.push_back(Tail);
    }

    // Присваиваем записанные данные элементам
    for (ListNode*& Node : Nodes)
    {
        size_t StrLen;
        fread(&StrLen, sizeof(StrLen), 1, Stream);
        Node->Data.resize(StrLen);
        fread(&Node->Data[0], sizeof(char), StrLen, Stream);
        
        int RandNodeId;
        fread(&RandNodeId, sizeof(RandNodeId), 1, Stream);
        if (RandNodeId != -1)
        {
            Node->Rand = Nodes.at(RandNodeId);
        }
    }
}

void ListRand::ToHashmap(std::unordered_map<ListNode*, int>& OutHashmap) const
{
    OutHashmap.clear();
    
    ListNode* Node = Head;
    for (int i = 0; i < Count; ++i)
    {
        OutHashmap.insert(std::make_pair(Node, i));
        Node = Node->Next;
    }
}
