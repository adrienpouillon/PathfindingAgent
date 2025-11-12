#include "Agent.h"
#include "Node.h"
#include "Grid.h"


template<typename T>
inline void Agent<T>::OnUpdate(float deltatime)
{
	UpdatePath();
}

template<typename T>
inline void Agent<T>::UpdatePath()
{
	if (mPath.GetPathFinish())
	{
		return;
	}

	if (GetTarget().isSet)
	{
		return;
	}

	std::vector<Node<T>*>* allPaths = mPath.GetPath();
	T* cell = (*allPaths)[mIndexPath]->GetData();
	sf::Vector2f pos = cell->getPosition();
	GoToPosition(pos.x, pos.y);
	mIndexPath++;
	if (mIndexPath == allPaths->size())
	{
		mPath.SetPathFinish(true);
		mIndexPath = 0;

		if (GetRoam())
		{
			GoToNode(mPath.GetStartNode());
		}
	}
}

template<typename T>
inline void Agent<T>::GoToCell(T* cellEnd, std::vector<Node<T>*> allNodes)
{
	GoToCell(cellEnd.getPosition(), allNodes);
}

template<typename T>
inline void Agent<T>::GoToCell(sf::Vector2f pos, std::vector<Node<T>*> allNodes)
{
	GoToNode(Node<T>::GetNodeInTab(pos.x, pos.y, allNodes.size(), allNodes));
}

template<typename T>
inline void Agent<T>::GoToNode(Node<T>* nodeEnd)
{
	//sf::Vector2f currentPos = GetPosition();
	//MainScene* mainScene = GameManager::Get()->GetScene<MainScene>();

	Grid<T>* grid = GameManager::Get()->GetScene<MainScene>()->GetGrid();
	
	//std::vector<Node<T>> allNodes = grid->GetAllNodes();
	//int cellSize = grid->GetCellSize();

	//sf::Vector2f index = Utils::GetCellInPos(GetPosition(), grid->GetCellSize());

	//Node<T>* nodeStart = Node<T>::GetNodeInTab(index.x, index.y, allNodes.size(), allNodes);
	Node<T>* nodeStart = GetCurrentNode(grid);
	mPath.PathBegin(nodeStart, nodeEnd);
	mPath.FindPath();
}

template<typename T>
T* Agent<T>::GetCurrentCell(Grid<T>* grid)
{
	sf::Vector2f index = Utils::GetCellInPos(GetPosition(), grid->GetCellSize());
	return grid->GetAllCells()[index.x][index.y];
}

template<typename T>
Node<T>* Agent<T>::GetCurrentNode(Grid<T>* grid)
{
	sf::Vector2f index = Utils::GetCellInPos(GetPosition(), grid->GetCellSize());
	std::vector<Node<T>> allNodes = grid->GetAllNodes();
	return Node<T>::GetNodeInTab(index.x, index.y, allNodes.size(), &allNodes);
}
