#include "PathFinding.h"
#include "../Utils.h"
#include "Cursor.h"

template<typename T>
void PathFinding<T>::InitQueue()
{
    ResetQueue();
    mQueue.push(GetStartNode());
}

template<typename T>
void PathFinding<T>::ResetQueue()
{
    for (int i = 0; i < mQueue.size(); i++)
    {
        mQueue.pop();
    }
}

template<typename T>
void PathFinding<T>::PathBegin(Node<T>* startNode, Node<T>* endNode)
{
    SetStartNode(startNode);
    SetEndNode(endNode);

    InitQueue();
    SetPathFinish(false);
    SetPath(std::vector<Node<T>*>());

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

        nodeCurrent->SetVisited(true);

        int disStart = nodeCurrent->GetDisStart();

        std::vector<Node<T>*> neighbor = nodeCurrent->GetNeighbor();

        for (Node<T>* nodeN : neighbor)
        {
            if (nodeN->GetVisited() == false)
            {
                nodeN->SetDisStart(disStart + 1);
                nodeN->SetDisEnd(Utils::DisManhattan(nodeN->GetData()->getPosition(), endPosition));
                nodeN->SetCallMe(nodeCurrent);

                mQueue.push(nodeN);
            }
        }
    }
    SetPath(std::vector<Node<T>*>());
}

template<typename T>
void PathFinding<T>::FindPathWithDebug()
{
    Node<T>* endNode = GetEndNode();
    if((int)mQueue.size() != 0)
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
            ResetQueue();
            return;
        }

        T* endCell = endNode->GetData();
        sf::Vector2f endPosition = endCell->getPosition();

        nodeCurrent->SetVisited(true);

        int disStart = nodeCurrent->GetDisStart();

        std::vector<Node<T>*> neighbor = nodeCurrent->GetNeighbor();

        for (Node<T>* nodeN : neighbor)
        {
            if (nodeN->GetVisited() == false)
            {
                nodeN->SetDisStart(disStart + 1);
                nodeN->SetDisEnd(Utils::DisManhattan(nodeN->GetData()->getPosition(), endPosition));
                nodeN->SetCallMe(nodeCurrent);

                mQueue.push(nodeN);
            }
        }
    }
    else
    {
        if (GetPath()->size() == 0)
        {
            SetPath(std::vector<Node<T>*>());
        }
    }
}

template<typename T>
void PathFinding<T>::CreatePath()
{
    Node<T>* endNode = mEndNode;
    std::vector<Node<T>*> allPathEnds = std::vector<Node<T>*>();
    while (endNode != mStartNode)
    {
        allPathEnds.push_back(endNode);
        endNode = endNode->GetCallMe();
    }

    std::vector<Node<T>*> allFormerPaths = *GetPath();
    std::vector<Node<T>*> allPathStarts;
    if (GetPathFinish())
    {
        allPathStarts = std::vector<Node<T>*>();
    }
    else
    {
        allPathStarts = allFormerPaths;
    }
    
    int lenghtAllPathEnds = allPathEnds.size() - 1;
    for (int i = lenghtAllPathEnds; i > -1; i--)
    {
        allPathStarts.push_back(allPathEnds[i]);
    }

    SetPath(allPathStarts);
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
