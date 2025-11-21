#include "Functions.h"

#include "../LightEngine/Utils.h"
#include "../LightEngine/Debug.h"
#include "Grid.h"
#include "Node.h"
#include "Compare.h"
#include "Agent.h"

#include <queue>

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
	std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>> queue;
	startNode->SetDisStart(0.f);
	startNode->SetDisEnd(Utils::DisManhattan(startNode->GetData()->GetPosition(), endNode->GetData()->GetPosition()));
	queue.push(startNode);

	while (queue.empty() == false)
	{
		Node<T>* nodeCurrent = queue.top();
		queue.pop();

		T* currentNodeData = nodeCurrent->GetData();

		Agent* currentNodeAgent = currentNodeData->GetAgent();

		if (currentNodeData->HasAgent() == true && currentNodeAgent != pOwner) // Ignore if agent and not owner
		{
			if(pOwner->IsGhost() && currentNodeAgent->IsGhost() == false)
			{
				// Allowed to pass through it
			}
			else
			{
				continue;
			}
		}

		if (nodeCurrent->GetVisited() == true)
		{
			continue;
		}

		if (nodeCurrent == endNode)
		{
			CreatePath(false, startNode, endNode, allFormerPaths);
			return;
		}

		nodeCurrent->SetVisited(true);

		gce::Vector3f32 endPosition = endNode->GetData()->GetPosition();

		for (NeighborsCost<T>& nodeNC : nodeCurrent->GetNeighborsCost())
		{
			Node<T>* nodeN = nodeNC.GetNeighbor();

			T* tNeighbor = nodeN->GetData();

			float minimalDist = nodeCurrent->GetDisStart() + nodeNC.GetCost(); 

			if (minimalDist < nodeN->GetDisStart())
			{
				nodeN->SetDisStart(minimalDist);
				nodeN->SetDisEnd(Utils::DisManhattan(nodeN->GetData()->GetPosition(), endPosition));
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

	int lenghtAllPathEnds = allPathEnds.size() - 1;
	for (int i = lenghtAllPathEnds; i > -1; i--)
	{
		allPathStarts->push_back(allPathEnds[i]);
	}
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
inline T* GetCell(gce::Vector3f32 pos, Grid* grid)
{
	gce::Vector2i32 index = Utils::GetCellInPos(pos, grid->GetCellSize());
	return grid->GetAllCells()[index.x][index.y];
}

template<typename T>
inline Node<T>* GetNode(gce::Vector3f32 pos, Grid* grid)
{
	gce::Vector2i32  index = Utils::GetCellInPos(pos, grid->GetCellSize());
	std::vector<Node<T>*> allNodes = grid->GetAllNodes();
	return GetNodeInTab(index.x, index.y, grid->GetAllCells()[0].size(), &allNodes);
}

template<typename T>
inline Node<T>* GetNodeInTab(int row, int col, int length_col, std::vector<Node<T>*>* allNodes)
{
	return (*allNodes)[row * length_col + col];
}

template<typename T>
inline T* GetNearestCell(gce::Vector3f32 pos, std::vector<std::vector<T*>> allCells)
{
	T* nearest = nullptr;
	float smallestSquaredDist = INT_MAX;

	for (auto& row : allCells)
	{
		for (auto cell : row)
		{
			float dx = abs(cell->GetPosition().x - pos.x);
			float dy = abs(cell->GetPosition().z - pos.z);

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
inline T* GetEntityNearest(gce::Vector3f32 pos, float minDis)
{
	std::list<Entity*> allEntities = GameManager::Get()->GetEntities();

	gce::Vector3f32 posNearest = gce::Vector3f32(minDis, minDis, minDis);
	T* entityNearest = nullptr;

	for (Entity* e : allEntities)
	{
		if (T* c = dynamic_cast<T*>(e))
		{
			gce::Vector3f32 posC = c->GetPosition();
			if (abs(pos.x - posC.x) <= posNearest.x && abs(pos.y - posC.y) <= posNearest.y && abs(pos.z - posC.z) <= posNearest.z)
			{
				posNearest = posC;
				entityNearest = c;
			}
		}
	}

	return entityNearest;
}
