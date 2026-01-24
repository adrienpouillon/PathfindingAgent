#include "Functions.h"

#include "../Utils.h"
#include "../Debug.h"
#include "Grid.h"
#include "Node.h"
#include "Compare.h"
#include "../Entity.h"
#include "Agent.h"

#include <queue>
#include <iostream>

template<typename A>
inline void AddVector2OnVector1(std::vector<A>* vector1, const std::vector<A>& vector2)
{
    int lenghtVector2 = vector2.size();
    for (int i = 0; i < lenghtVector2; i++)
    {
        vector1->push_back(vector2[i]);
    }
}

template<typename A>
inline void AddReveseVector2OnVector1(std::vector<A>* vector1, const std::vector<A>& vector2)
{
    int lenghtVector2 = vector2.size() - 1;
    for (int i = lenghtVector2; i > -1; i--)
    {
        vector1->push_back(vector2[i]);
    }
}

template<typename T>
void FindPath(Node<T>* startNode, Node<T>* endNode, Grid* grid, std::vector<Node<T>*>* allFormerPaths, Agent* pOwner)
{
	ResetNodes<T>(grid);
	int sizeCell = grid->GetCellSize();

	std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>> queue;
	startNode->SetDisStart(0.f);
	startNode->SetDisEnd(Utils::DisManhattan(startNode->GetData()->getPosition(), endNode->GetData()->getPosition(), sizeCell));
	queue.push(startNode);

	while (queue.empty() == false)
	{
		Node<T>* nodeCurrent = queue.top();
		queue.pop();

		T* currentNodeData = nodeCurrent->GetData();

		Agent* currentNodeAgent = currentNodeData->GetAgent();

		if (currentNodeData->HasAgent() == true && currentNodeAgent != pOwner) // Ignore if agent and not owner
		{
			if (pOwner->IsGhost() && currentNodeAgent->IsGhost() == false)
			{
				// Allowed to pass through it
			}
			else
			{
				continue;
			}
		}

		if (nodeCurrent == endNode)
		{
			int length = (int)queue.size();

			if (queue.empty() == false)
			{
				std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>> queu = queue;
				for (int i = 0; i < length; i++)
				{
					Node<T>* node = queu.top();
					T* cell = node->GetData();
					sf::Vector2f nodePosition = cell->getPosition();
					Debug::DrawCircle(nodePosition.x, nodePosition.y, 5.f, sf::Color::White);
					queu.pop();
				}
			}
			CreatePath(false, startNode, endNode, allFormerPaths);
			return;
		}

		nodeCurrent->SetVisited(true);

		sf::Vector2f endPosition = endNode->GetData()->getPosition();

		for (NeighborsCost<T>& structNeighborCost : nodeCurrent->GetNeighborsCost())
		{
			Node<T>* nodeN = structNeighborCost.GetNeighbor();
			T* cell = nodeN->GetData();
		
			float minimalDist = nodeCurrent->GetDisStart() + structNeighborCost.GetCost(); // f = g + h
			if (minimalDist < nodeN->GetDisStart())
			{
				float valueCoin = (float)(cell->HasCoin());
				float disEnd = Utils::DisManhattan(cell->getPosition(), endPosition, sizeCell);

				nodeN->SetDisStart(minimalDist);
				nodeN->SetDisEnd(disEnd - valueCoin);
				nodeN->SetCallMe(nodeCurrent);

				queue.push(nodeN);
			}
		}
	}

	allFormerPaths->clear();
}

template<typename T>
void CreatePath(bool isfinish, Node<T>* startNode, Node<T>* endNode, std::vector<Node<T>*>* allFormerPaths)
{
	std::vector<Node<T>*> allPathEnds = std::vector<Node<T>*>();
	while (endNode != startNode)
	{
		allPathEnds.push_back(endNode);
		endNode = endNode->GetCallMe();
	}

	std::vector<Node<T>*>* allPathStarts = allFormerPaths;
	if (isfinish)
	{
		*allPathStarts = std::vector<Node<T>*>();
	}

    AddReveseVector2OnVector1<Node<T>*>(allPathStarts, allPathEnds);
}

template<typename T>
void ResetNodes(Grid* grid)
{
   	std::vector<Node<T>*> allNodes = grid->GetAllNodes();
	int lenght = allNodes.size();
	for (int i = 0; i < lenght; i++)
	{
		allNodes[i]->SetDisStart(INT_MAX);
		allNodes[i]->SetVisited(false);
		allNodes[i]->SetCallMe(nullptr);
	}
}

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

template<typename T>
inline T* GetCell(sf::Vector2f pos, Grid* grid)
{
	sf::Vector2f index = Utils::GetCellInPos(pos, grid->GetCellSize());
	return grid->GetAllCells()[index.x][index.y];
}

template<typename T>
inline Node<T>* GetNode(sf::Vector2f pos, Grid* grid)
{
	sf::Vector2f index = Utils::GetCellInPos(pos, grid->GetCellSize());
	std::vector<Node<T>*> allNodes = grid->GetAllNodes();
	return GetNodeInTab(index.x, index.y, grid->GetAllCells()[0].size(), &allNodes);
}

template<typename T>
inline Node<T>* GetNodeInTab(int row, int col, int length_col, std::vector<Node<T>*>* allNodes)
{
	return (*allNodes)[row * length_col + col];
}

template<typename T>
inline T* GetNearestCell(sf::Vector2f pos, std::vector<std::vector<T*>> allCells)
{
	T* nearest = nullptr;
	float smallestSquaredDist = INT_MAX;

	for (auto& row : allCells)
	{
		for (auto cell : row)
		{
			float dx = abs(cell->getPosition().x - pos.x);
			float dy = abs(cell->getPosition().y - pos.y);

			float squaredDist = dx * dx + dy * dy;

			if (squaredDist < smallestSquaredDist)
			{
				nearest = cell;

				smallestSquaredDist = squaredDist;
			}
		}
	}

	return nearest;
}

template<typename T>
inline T* GetEntityNearest(sf::Vector2f pos, float minDis)
{
	std::list<Entity*> allEntities = GameManager::Get()->GetEntities();

	sf::Vector2f posNearest = sf::Vector2f(minDis, minDis);
	T* entityNearest = nullptr;

    for (Entity* e : allEntities)
    {
        if (T* c = dynamic_cast<T*>(e))
        {
            sf::Vector2f posC = c->GetPosition();
            if (abs(pos.x - posC.x) <= posNearest.x && abs(pos.y - posC.y) <= posNearest.y)
            {
                posNearest = posC;
                entityNearest = c;
            }
        }
    }

	return entityNearest;
}