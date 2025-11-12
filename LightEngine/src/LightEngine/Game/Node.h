#pragma once
#include <vector>
#include "Cell.h"

template<typename T>
class Node
{
protected:
    T* mData;
    bool mVisited;
    Node* mCallMe;
    std::vector<Node<T>*> mAllNeighbors;
    int mDisStart;
    int mDisEnd;
public:
    Node() { Start(); }

    void Start() { SetData(nullptr); SetVisited(false); SetCallMe(nullptr); SetNeighbor(std::vector<Node<T>*>()); SetDisStart(0); SetDisEnd(0); }

    static Node<T>* GetNodeInTab(int i, int j, int lenght_i, std::vector<Node<T>*>* allNodes);

    void SetAll(T* data, bool visited, Node<T>* callMe, std::vector<Node<T>*> neighbor, int disStart, int disEnd)
    {
        SetData(data); SetVisited(visited); SetCallMe(callMe); SetNeighbor(neighbor); SetDisStart(disStart); SetDisEnd(disEnd);
    }

    void SetVisited(bool visited) { mVisited = visited; }
    bool GetVisited() { return mVisited; }

    void SetData(T* data) { mData = data; }
    T* GetData() { return mData; }



    void SetCallMe(Node<T>* callMe) { mCallMe = callMe; }
    Node<T>* GetCallMe() { return mCallMe; }

    void SetNeighbor(std::vector<Node<T>*> neighbor) { mAllNeighbors = neighbor; }
    void SetFourNeighbor(Node<T>* neighbor1, Node<T>* neighbor2, Node<T>* neighbor3, Node<T>* neighbor4) { mAllNeighbors.push_back(neighbor1); mAllNeighbors.push_back(neighbor2); mAllNeighbors.push_back(neighbor3); mAllNeighbors.push_back(neighbor4); }
    std::vector<Node*> GetNeighbor() { return mAllNeighbors; }

    void SetDisStart(int dis) { mDisStart = dis; }
    int GetDisStart() { return mDisStart; }

    void SetDisEnd(int dis) { mDisEnd = dis; }
    int GetDisEnd() { return mDisEnd; }

    ~Node() {}
};

#include "Node.inl"
