#include "Agent.h"

#include "../Debug.h"

#include "MainScene.h"
#include "Node.h"
#include "Grid.h"
#include "Functions.h"

void Agent::OnUpdate()
{
	UpdatePath();
}

void Agent::UpdatePath()
{
	std::vector<Node<Cell>*>* allPaths = mPath.GetPath();
	if (allPaths->size() < 1)
	{
		return;
	}
	int lenghtAllPaths = allPaths->size() - 1;
	for (int i = mIndexPath; i < lenghtAllPaths; i++)
	{
		Cell* currentCell = (*allPaths)[i]->GetData();
		Cell* nextCell = (*allPaths)[i + 1]->GetData();

		sf::Vector2f currentPos = currentCell->getPosition();
		sf::Vector2f nextPos = nextCell->getPosition();

		Debug::DrawLine(currentPos.x, currentPos.y, nextPos.x, nextPos.y, sf::Color::Yellow);
	}

	if (GetTarget().isSet)
	{
		return;
	}

	Cell* currentCell = (*allPaths)[mIndexPath]->GetData();
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

void Agent::GoToCell(Cell* cellEnd, Grid* grid)
{
	GoToCell(cellEnd->getPosition(), grid);
}

void Agent::GoToCell(sf::Vector2f pos, Grid* grid)
{
	GoToNode(GetNode<Cell>(pos, grid), grid);
	//::GetNodeInTab(pos.x, pos.y, allNodes.size(), allNodes));
}

void Agent::GoToNode(Node<Cell>* endNode, Grid* grid)
{
	Node<Cell>* startNode;
	std::vector<Node<Cell>*>* currentePath = mPath.GetPath();
	int lenght = currentePath->size();
	if (lenght > 0)
	{
		//position final du path precedent
		startNode = (*currentePath)[lenght - 1];
	}
	else
	{
		//position actuelle
		startNode = GetNode<Cell>(GetPosition(), grid);
	}
	SetStartNode(startNode);
	SetEndNode(endNode);
	mPath.Find(startNode, endNode, grid);
}
