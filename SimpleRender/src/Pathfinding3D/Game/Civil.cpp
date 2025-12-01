#include "pch.h"
#include "Civil.h"
#include "MainScene.h"
#include "Functions.h"

void Civil::OnUpdate()
{
	Agent::OnUpdate();

	if (mpCurrentScene->GetUseCoin())
	{
		int pathSize = mPath.GetPath()->size();
		if (pathSize > 0)
		{
			int cost = 10;
			if (mCoin >= cost)
			{
				float cellSize = mpCurrentScene->GetGrid()->GetCellSize();
				float agentSize = cellSize * 0.5f;

				int ran = GetRandomNumber(0, 10);
				if (ran < 5)
				{
					mpCurrentScene->CreateGuard(GetPosition(), 100.f, agentSize, gce::Vector3f32(0.f, 1.f, 1.f), gce::Vector3f32(0.f, 0.f, 1.f));
					IncreaseCoin(-cost);
				}
				else if (ran > 5)
				{
					mpCurrentScene->CreateCivil(GetPosition(), 75.f, agentSize, gce::Vector3f32(1.f, 0.5f, 0.f), gce::Vector3f32(1.f, 0.f, 0.f));
					IncreaseCoin(-cost);
				}
				else
				{
					mpCurrentScene->CreateAssassin(GetPosition(), 100.f, agentSize, gce::Vector3f32(1.f, 0.f, 0.f), gce::Vector3f32(1.f, 0.f, 1.f));
					IncreaseCoin(-cost);
				}
			}
		}
	}
}

void Civil::OnPathFinish()
{
	Agent::OnPathFinish();

	Grid* grid = mpCurrentScene->GetGrid();

	std::vector<Node<Cell>*> allNodes = grid->GetAllNodes();
	int lenghtRow = grid->GetAllCells().size();
	int lenghtCol = grid->GetAllCells()[0].size();
	int row = GetRandomNumber(0, lenghtRow - 1);
	int col = GetRandomNumber(0, lenghtCol - 1);

	Node<Cell>* node = GetNodeInTab(row, col, lenghtCol, &allNodes);
	if (node->GetData()->HasAgent() == false && node->GetData()->IsObstacle() == false)
	{
		GoToNode(node, grid);
	}
}
