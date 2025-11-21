#pragma once
#include "Node.h"

template<typename T>
struct CompareBFS
{
    bool operator()(Node<T>* a, Node<T>* b)
    {
        return a->GetDisStart() > b->GetDisStart();
    }
};

template<typename T>
struct CompareDFS
{
    bool operator()(Node<T>* a, Node<T>* b)
    {
        return a->GetDisEnd() > b->GetDisEnd();
    }
};

template<typename T>
struct CompareASTAR
{
    bool operator()(Node<T>* a, Node<T>* b)
    {
        return a->GetDisStart() + a->GetDisEnd() > b->GetDisStart() + b->GetDisEnd();
    }
};

