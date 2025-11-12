#include "Agent.h"
#include "Node.h"

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
	GoToNode(GetNodeInTab(pos.x, pos.y, allNodes.size(), allNodes));
}

template<typename T>
inline void Agent<T>::GoToNode(Node<T>* nodeEnd)
{
	sf::Vector2f startPos = GetPosition();
	mPath.PathBegin(startPos, nodeEnd);
	mPath.FindPath();
}
