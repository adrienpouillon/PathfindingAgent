#include "Agent.h"
#include "Node.h"
#include "Grid.h"
#include "../Debug.h"

template<typename T>
void Agent<T>::OnUpdate()
{
	std::vector<Node<T>*>* allPaths = mPath.GetPath();
	mIndexPath = mIndexPath;
	mPath.mFinish = mPath.mFinish;
	mPath.mQueue = mPath.mQueue;
	mPath.mStartNode = mPath.mStartNode;
	mPath.mEndNode = mPath.mEndNode;
	mRoam = mRoam;
	//mPath.FindPathWithDebug();
	UpdatePath();
}

template<typename T>
void Agent<T>::UpdatePath()
{
	if (mPath.GetPathFinish())
	{
		return;
	}

	std::vector<Node<T>*>* allPaths = mPath.GetPath();
	int lenghtAllPaths = allPaths->size() - 1;

	if (mIndexPath >= lenghtAllPaths)
	{
		return;
	}

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

	//std::vector<Node<T>*>* allPaths = mPath.GetPath();
	T* currentCell = (*allPaths)[mIndexPath]->GetData();
	sf::Vector2f currentPos = currentCell->getPosition();
	GoToPosition(currentPos.x, currentPos.y);
	mIndexPath++;
	if (mIndexPath == allPaths->size())
	{
		Node<T>* startNode = mPath.GetStartNode();
		mPath.SetPathFinish(true);
		mPath.SetPath(std::vector<Node<T>*>());
		mIndexPath = 0;

		if (GetRoam())
		{
			CGrid* grid = GameManager::Get()->GetScene<MainScene>()->GetGrid();
			GoToNode(startNode, grid);
		}
	}
}

template<typename T>
void Agent<T>::GoToCell(T* cellEnd, CGrid* grid)
{
	GoToCell(cellEnd.getPosition(), grid);
}

template<typename T>
void Agent<T>::GoToCell(sf::Vector2f pos, CGrid* grid)
{
	GoToNode(Utils::GetNode(pos, grid), grid);
	//::GetNodeInTab(pos.x, pos.y, allNodes.size(), allNodes));
}

template<typename T>
void Agent<T>::GoToNode(Node<T>* nodeEnd, CGrid* grid)
{
	Node<T>* nodeStart = Utils::GetNode(GetPosition(), grid);
	mPath.PathBegin(nodeStart, nodeEnd);
	mPath.FindPath();
}
