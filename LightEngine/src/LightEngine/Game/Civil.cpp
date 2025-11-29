#include "Civil.h"

#include "MainScene.h"
#include "Functions.h"

void Civil::OnUpdate()
{
	Agent::OnUpdate();

	if (mCurrentScene->GetUseCoin())
	{
		int pathSize = mPath.GetPath()->size();
		if (pathSize > 0)
		{
			int cost = 10;
			if (mCoin >= cost)
			{
				int ran = GetRandomNumber(0, 10);
				if (ran < 5)
				{
					mCurrentScene->CreateGuard(GetPosition(), 100.f, RADIUS, sf::Color::Cyan, sf::Color::Blue);
					IncreaseCoin(-cost);
				}
				else if (ran > 5)
				{
					mCurrentScene->CreateCivil(GetPosition(), 75.f, RADIUS, sf::Color::Yellow, sf::Color(64, 128, 128));
					IncreaseCoin(-cost);
				}
				else
				{
					mCurrentScene->CreateAssassin(GetPosition(), 100.f, RADIUS, sf::Color::Red, sf::Color::Magenta);
					IncreaseCoin(-cost);
				}
			}
		}
	}
}

void Civil::OnPathFinish()
{
	Agent::OnPathFinish();

	Grid* grid = mCurrentScene->GetGrid();

	std::vector<Node<Cell>*> allNodes = grid->GetAllNodes();
	int lenghtRow = grid->GetAllCells().size();
	int lenghtCol = grid->GetAllCells()[0].size();
	int row = GetRandomNumber(0, lenghtRow - 1);
	int col = GetRandomNumber(0, lenghtCol - 1);

	Node<Cell>* node = GetNodeInTab(row, col, lenghtCol, &allNodes);
	if (node->GetData()->IsObstacle() == false)
	{
		GoToNode(node, grid);
	}
}
