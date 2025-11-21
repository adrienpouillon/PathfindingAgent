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
    float mDisStart;
    float mDisEnd;
public:
    Node() { Start(); }

    void Start() { SetData(nullptr); SetVisited(false); SetCallMe(nullptr); SetNeighborsCost(std::vector<NeighborsCost<T>>()); SetDisStart(INT_MAX); SetDisEnd(0); }

    void SetAll(T* data, bool visited, Node<T>* callMe, std::vector<NeighborsCost<T>> neighbor, float disStart, float disEnd)
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

    void SetDisStart(float dis) { mDisStart = dis; }
    float GetDisStart() { return mDisStart; }

    void SetDisEnd(float dis) { mDisEnd = dis; }
    float GetDisEnd() { return mDisEnd; }

};

template<typename T>
struct NeighborsCost
{
    Node<T>* mNeighbor;
    float mCost;

    NeighborsCost() { mNeighbor = nullptr; mCost = 0.f; }
    NeighborsCost(Node<T>* neighbor, float cost) { mNeighbor = neighbor; mCost = cost; }

    void SetCost(float cost) { mCost = cost; }
    float GetCost() { return mCost; }

    void SetNeighbor(Node<T>* neighbor) { mNeighbor = neighbor; }
    Node<T>* GetNeighbor() { return mNeighbor; }
};

#include "Node.inl"
