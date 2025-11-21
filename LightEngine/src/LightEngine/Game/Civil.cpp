#include "Civil.h"

#include "MainScene.h"
#include "Functions.h"

void Civil::OnUpdate()
{
	Agent::OnUpdate();
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

int Civil::GetRandomNumber(int min, int max)
{
    int range = max - min + 1;
    int value = rand() % range + min;
    return value;
}
