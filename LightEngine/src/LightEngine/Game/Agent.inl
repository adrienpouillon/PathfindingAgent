#include "Agent.h"
#include "Node.h"
#include "Grid.h"
#include "../Debug.h"
#include "Functions.h"


template<typename T>
void Agent<T>::OnUpdate()
{
	UpdatePath();
}

template<typename T>
void Agent<T>::UpdatePath()
{
	std::vector<Node<T>*>* allPaths = mPath.GetPath();
	if (allPaths->size() < 1)
	{
		return;
	}
	int lenghtAllPaths = allPaths->size() - 1;
	for (int i = mIndexPath; i < lenghtAllPaths; i++)
	{
		T* currentCell = (*allPaths)[i]->GetData();
		T* nextCell = (*allPaths)[i + 1]->GetData();

		sf::Vector2f currentPos = currentCell->getPosition();
		sf::Vector2f nextPos = nextCell->getPosition();

		Debug::DrawLine(currentPos.x, currentPos.y, nextPos.x, nextPos.y, sf::Color::Yellow);
	}

	if (GetTarget().isSet)
	{
		return;
	}

	T* currentCell = (*allPaths)[mIndexPath]->GetData();
	sf::Vector2f currentPos = currentCell->getPosition();
	GoToPosition(currentPos.x, currentPos.y);
	mIndexPath++;
	if (mIndexPath == allPaths->size())
	{
		mPath.SetReadFinish(true);
		mIndexPath = 0;

		if (GetRoam())
		{
			Grid* grid = GameManager::Get()->GetScene<MainScene>()->GetGrid();
			GoToNode(GetStartNode(), grid);
		}
	}
}

template<typename T>
void Agent<T>::GoToCell(T* cellEnd, Grid* grid)
{
	GoToCell(cellEnd.getPosition(), grid);
}

template<typename T>
void Agent<T>::GoToCell(sf::Vector2f pos, Grid* grid)
{
	GoToNode(GetNode(pos, grid), grid);
	//::GetNodeInTab(pos.x, pos.y, allNodes.size(), allNodes));
}

template<typename T>
void Agent<T>::GoToNode(Node<T>* endNode, Grid<T>* grid)
{
	Node<T>* startNode;
	std::vector<Node<T>*>* currentePath = mPath.GetPath();
	int lenght = currentePath->size();
	if (lenght > 0)
	{
		//position final du path precedent
		startNode = (*currentePath)[lenght - 1];
	}
	else
	{
		//position actuelle
		startNode = GetNode(GetPosition(), grid);
	}
	SetStartNode(startNode);
	SetEndNode(endNode);
	mPath.Find(startNode, endNode, grid);
}
