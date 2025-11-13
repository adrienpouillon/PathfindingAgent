#include "../Utils.h"
#include "Functions.h"
#include "Grid.h"
#include "Node.h"

template<typename T>
inline T* GetCell(sf::Vector2f pos, Grid<T>* grid)
{
	sf::Vector2f index = GetCellInPos(pos, grid->GetCellSize());
	return grid->GetAllCells()[index.x][index.y];
}

template<typename T>
inline Node<T>* GetNode(sf::Vector2f pos, Grid<T>* grid)
{
	sf::Vector2f index = GetCellInPos(pos, grid->GetCellSize());
	std::vector<Node<T>*> allNodes = grid->GetAllNodes();
	return Node<T>::GetNodeInTab(index.x, index.y, grid->GetAllCells().size(), &allNodes);
}

template<typename T>
inline Node<T>* GetNodeInTab(int i, int j, int lenght_i, std::vector<Node<T>*>* allNodes)
{
	return (*allNodes)[i * lenght_i + j];
}

template<typename T>
void ResetNodes(Grid<T>* grid)
{
    std::vector<Node<T>*> allNodes = grid->GetAllNodes();
    int lenght = allNodes.size();
    for (int i = 0; i < lenght; i++)
    {
        allNodes[i]->SetVisited(false);
        allNodes[i]->SetCallMe(nullptr);
    }
}

template<typename T>
std::vector<Node<T>*> FindPath(Node<T>* startNode, Node<T>* endNode, Grid<T>* grid, std::vector<Node<T>*> allFormerPaths)
{

    ResetNodes(grid);
    std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>> queue = std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>>();
    queue.push(startNode);
    startNode->SetDisStart(0);
    startNode->SetDisEnd(Utils::DisManhattan(startNode->GetData()->getPosition(), endNode->GetData()->getPosition()));

    while ((int)queue.size() != 0)
    {
        Node<T>* nodeCurrent = queue.top();
        queue.pop();

        if (nodeCurrent->GetVisited() == true)
        {
            continue;
        }

        if (nodeCurrent == endNode)
        {
            return CreatePath(false, startNode, endNode, allFormerPaths);
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

                queue.push(nodeN);
            }
        }
    }
    return std::vector<Node<T>*>();
}

template<typename T>
std::vector<Node<T>*> CreatePath(bool isfinish, Node<T>* startNode, Node<T>* endNode, std::vector<Node<T>*> allFormerPaths)
{
    std::vector<Node<T>*> allPathEnds = std::vector<Node<T>*>();
    while (endNode != startNode)
    {
        allPathEnds.push_back(endNode);
        endNode = endNode->GetCallMe();
    }

    //std::vector<Node<T>*> allFormerPaths = *GetPath();
    std::vector<Node<T>*> allPathStarts;
    if (isfinish)
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

    return allPathStarts;
}

/*template<typename T>
void PathFinding<T>::FindPathWithDebug()
{
    Node<T>* endNode = GetEndNode();
    int lenght = (int)mQueue.size();
    if (lenght != 0)
    {
        std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>> queue = mQueue;
        for (int i = 0; i < lenght; i++)
        {
            Node<T>* node = queue.top();
            T* cell = node->GetData();
            sf::Vector2f nodePosition = cell->getPosition();
            Debug::DrawCircle(nodePosition.x, nodePosition.y, 5.f, sf::Color::White);
            queue.pop();
        }

        Node<T>* nodeCurrent = mQueue.top();
        mQueue.pop();

        if (nodeCurrent->GetVisited() == true)
        {
            return;
        }

        if (nodeCurrent == endNode)
        {
            CreatePath();
            ResetQueue();
            return;
        }

        T* endCell = endNode->GetData();
        sf::Vector2f endPosition = endCell->getPosition();
        Debug::DrawCircle(endPosition.x, endPosition.y, 10.f, sf::Color::Red);

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
*/


template<typename A>
inline std::vector<A*>* EraseTab(std::vector<A*>* all)
{
    int lenghtAll = all->size() - 1;
    for (int i = lenghtAll; i > -1; i--)
    {
        delete (*all)[i];
    }
    return all;
}


template<typename A>
inline std::vector<std::vector<A*>>* EraseTab(std::vector<std::vector<A*>>* all)
{
    int lenghtAllI = all->size() - 1;
    int lenghtAllJ = all[0].size() - 1;
    for (int i = lenghtAllI; i > -1; i--)
    {
        for (int j = lenghtAllJ; j > -1; i--)
        {
            delete (*all)[i][j];
        }
        all[0].clear();
    }
    all->clear();
    return all;
}
