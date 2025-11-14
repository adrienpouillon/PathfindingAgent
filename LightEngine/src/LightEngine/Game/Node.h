#pragma once
#include <vector>
#include "Cell.h"

template<typename T>
struct NeighborsCost;

template<typename T>
class Node
{
protected:
    T* mData;
    bool mVisited;
    Node* mCallMe;
    std::vector<NeighborsCost<T>> mAllNeighborsCost;
    int mDisStart;
    int mDisEnd;
public:
    Node() { Start(); }

    void Start() { SetData(nullptr); SetVisited(false); SetCallMe(nullptr); SetNeighborsCost(std::vector<NeighborsCost<T>>()); SetDisStart(0); SetDisEnd(0); }

    void SetAll(T* data, bool visited, Node<T>* callMe, std::vector<NeighborsCost<T>> neighbor, int disStart, int disEnd)
    {
        SetData(data); SetVisited(visited); SetCallMe(callMe); SetNeighborsCost(neighbor); SetDisStart(disStart); SetDisEnd(disEnd);
    }

    void SetVisited(bool visited) { mVisited = visited; }
    bool GetVisited() { return mVisited; }

    void SetData(T* data) { mData = data; }
    T* GetData() { return mData; }

    void SetCallMe(Node<T>* callMe) { mCallMe = callMe; }
    Node<T>* GetCallMe() { return mCallMe; }

    void SetNeighborsCost(std::vector<NeighborsCost<T>> neighborCost) { mAllNeighborsCost = neighborCost; }
    std::vector<NeighborsCost<T>>& GetNeighborsCost() { return mAllNeighborsCost; }

    void SetDisStart(int dis) { mDisStart = dis; }
    int GetDisStart() { return mDisStart; }

    void SetDisEnd(int dis) { mDisEnd = dis; }
    int GetDisEnd() { return mDisEnd; }

    ~Node() {}
};

template<typename T>
struct NeighborsCost
{
    Node<T>* mNeighbor;
    int mCost;

    NeighborsCost() { mNeighbor = nullptr; mCost = 0; }
    NeighborsCost(Node<T>* neighbor, int cost) { mNeighbor = neighbor; mCost = cost; }

    void SetCost(int cost) { mCost = cost; }
    int GetCost() { return mCost; }

    void SetNeighbor(Node<T>* neighbor) { mNeighbor = neighbor; }
    Node<T>* GetNeighbor() { return mNeighbor; }
};

#include "Node.inl"
