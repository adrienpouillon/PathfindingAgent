#include "PathFinding.h"
#include "../Utils.h"
#include <iostream>

template<typename T>
PathFinding<T>::PathFinding()
{

}

template<typename T>
void PathFinding<T>::InitQueue()
{
    for (int i = 0; i < mQueue.size(); i++)
    {
        mQueue.pop();
    }

    mQueue.push(GetStartNode());
}

template<typename T>
void PathFinding<T>::PathBegin()
{
    Node<T>* startNode = GetStartNode();
    if (startNode == nullptr)
    {
        std::cout << "PathFinding : PathBegin() : mStartNode == nullptr";
        return;
    }

    Node<T>* endNode = GetEndNode();
    if (endNode == nullptr)
    {
        std::cout << "PathFinding : PathBegin() : mEndNode == nullptr";
        return;
    }

    InitQueue();
    SetPathFinish(false);

    startNode->SetDisStart(0);
    startNode->SetDisEnd(Utils::DisManhattan(startNode->GetData()->getPosition(), endNode->GetData()->getPosition()));

    mQueue.push(startNode);
}

template<typename T>
void PathFinding<T>::FindPath()
{
    Node<T>* endNode = GetEndNode();
    while ((int)mQueue.size() != 0)
    {
        Node<T>* nodeCurrent = mQueue.top();
        mQueue.pop();

        if (nodeCurrent->GetVisited() == true)
        {
            continue;
        }

        if (nodeCurrent == endNode)
        {
            CreatePath();
            return;
        }

        T* endCell = endNode->GetData();
        sf::Vector2f endPosition = endCell->getPosition();

        int endPosX = endPosition.x;
        int endPosY = endPosition.y;

        T* currentCell = nodeCurrent->GetData();
        sf::Vector2f currentPosition = currentCell->getPosition();

        int currentPosX = currentPosition.x;
        int currentPosY = currentPosition.y;



        nodeCurrent->SetVisited(true);

        int currentPosRight = currentPosX + 1;
        int currentPosLeft = currentPosX - 1;
        int currentPosDown = currentPosY + 1;
        int currentPosUp = currentPosY - 1;

        int disStart = nodeCurrent->GetDisStart();

        std::vector<Node<T>*> neighbor = nodeCurrent->GetNeighbor();

        for (Node<T>* nodeN : neighbor)
        {
            if (nodeN->GetVisited() == false)
            {
                nodeN->SetDisStart(disStart + 1);
                nodeN->SetDisEnd(Utils::DisManahattan(nodeN->GetData()->getPosition(), endPosition));
                nodeN->SetCallMe(nodeCurrent);

                mQueue.push(nodeN);
            }
        }
    }
    SetPath(std::vector<Node<T>*>());
}

template<typename T>
void PathFinding<T>::CreatePath()
{
    Node<T>* endNode = mEndNode;
    std::vector<Node<T>*> allPathEnd = std::vector<Node<T>*>();
    while (endNode != mStartNode)
    {
        allPathEnd.push_back(endNode);
        endNode = endNode->GetCallMe();
    }

    std::vector<Node<T>*> allPathStart = std::vector<Node<T>*>();
    int lenghtAllPathEnd = allPathEnd.size() - 1;
    for (int i = lenghtAllPathEnd; i > -1; i--)
    {
        allPathStart.push_back(allPathEnd[i]);
    }

    SetPath(allPathStart);
}

template<typename T>
void PathFinding<T>::SetPathFinish(bool finish)
{
    mFinish = finish;
    if (mFinish == true)
    {
        SetStartNode(nullptr);
        SetEndNode(nullptr);
    }
}

template<typename T>
bool PathFinding<T>::GetPathFinish()
{
    return mFinish;
}

template<typename T>
PathFinding<T>::~PathFinding()
{

}
