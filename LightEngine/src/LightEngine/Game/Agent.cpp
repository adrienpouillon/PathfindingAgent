#include "Agent.h"

#include "../Debug.h"
#include "../GameManager.h"
#include "MainScene.h"
#include "Node.h"
#include "Grid.h"
#include "Functions.h"
#include <iostream>

void Agent::OnInitialize()
{
	SetPathfinding(PathFinding<Cell>()); 
	mIndexPath = 0; 
	SetRoam(false); 

	SetStartNode(nullptr); 
	SetEndNode(nullptr); 

	SetClassicColor(sf::Color::White); 
	SetRoamColor(sf::Color::Black); 

	SetPosition(0.f, 0.f); 
	SetTag(AGENT);
	SetSpeed(1.f); 
	SetRigidBody(false);
	SetGhost(false);

	SetCurrentScene(GameManager::Get()->GetScene<MainScene>());
}

void Agent::OnUpdate()
{
	if (ToDestroy())
		return;

	UpdatePath();
}

void Agent::OnPathFinish()
{

}

void Agent::OnDestroy()
{
	if (mCurrentScene->GetSelectedEntity() == this)
	{
		mCurrentScene->SetSelectedEntity(nullptr);
	}
}

void Agent::UpdatePath()
{
	if (mCurrentScene == nullptr)
		return;

	Grid* grid = mCurrentScene->GetGrid();
	
	bool roam = GetRoam();
	if (roam)
	{
		GetShape()->setFillColor(mRoamColor);
	}
	else
	{
		GetShape()->setFillColor(mClassicColor);
	}

	std::vector<Node<Cell>*>* allPaths = mPath.GetPath();
	if (allPaths->size() < 1)
	{
		OnPathFinish();
		mIndexPath = 0;
		return;
	}

	if (mCurrentScene->GetRestartNode())
	{
		mNeedToWaitForReset = true;
	}

	if (mNeedToWaitForReset == true)
	{
		Node<Cell>* endNode = GetEndNode();
		ResetPath(endNode, grid);
	}

	int lenghtAllPaths = allPaths->size();
	bool haveGoodLenghtIndexPath = (mIndexPath == lenghtAllPaths);
	if (mCurrentScene->GetDrawDebug() != DRAW_DEBUG_NO)
	{
		if (!haveGoodLenghtIndexPath)
		{
			for (int i = mIndexPath; i < lenghtAllPaths - 1; i++)
			{
				Cell* currentCell = (*allPaths)[i]->GetData();
				Cell* nextCell = (*allPaths)[i + 1]->GetData();

				sf::Vector2f currentPos = currentCell->getPosition();
				sf::Vector2f nextPos = nextCell->getPosition();

				Debug::DrawLine(currentPos.x, currentPos.y, nextPos.x, nextPos.y, sf::Color::Yellow);
			}
		}
	}

	if (GetTarget().isSet)
	{
		return;
	}

	if (haveGoodLenghtIndexPath)
	{
		mPath.SetReadFinish(true);
		mIndexPath = 0;

		if (roam)
		{
			Node<Cell>* nodeEnd = GetStartNode();
			GoToNode(nodeEnd, grid);
		}
	}
	else
	{
		bool currentIsEmpty = false;
		if (mIndexPath - 1 >= 0)
		{
			Cell* currentCell = (*allPaths)[mIndexPath - 1]->GetData();
			currentIsEmpty = currentCell->GetAgent() != this;
		}
		Cell* nextCell = (*allPaths)[mIndexPath]->GetData();

		if (mWaitProgress <= mWaitTime && mIsGhost == false)
		{
			if (nextCell->HasAgent() || currentIsEmpty)
			{
				mWaitProgress += GetDeltaTime();
				return;
			}
		}
		if (mWaitProgress > mWaitTime && mIsGhost == false) // Waited too long
		{
			mNeedToWaitForReset = true;
		}
		else
		{
			sf::Vector2f nextPos = nextCell->getPosition();
			GoToPosition(nextPos.x, nextPos.y);
			mIndexPath++;
		}

		mWaitProgress = 0.f;
	}
}

void Agent::GoToCell(Cell* cellEnd, Grid* grid)
{
	sf::Vector2f indexRowCol = cellEnd->GetRowCol();
	std::vector<Node<Cell>*> allNodes = grid->GetAllNodes();
	int lengthCell = grid->GetCellSize();
	GoToNode(GetNodeInTab((int)indexRowCol.x, (int)indexRowCol.y, lengthCell, &allNodes), grid);
}

void Agent::GoToCell(sf::Vector2f pos, Grid* grid)
{
	GoToNode(GetNode<Cell>(pos, grid), grid);
}

void Agent::GoToNode(Node<Cell>* endNode, Grid* grid)
{
	Node<Cell>* startNode;
	std::vector<Node<Cell>*>* currentPath = mPath.GetPath();
	int length = currentPath->size();
	if (length > 0)
	{
		//position final du path precedent
		startNode = (*currentPath)[length - 1];
	}
	else
	{
		startNode = GetNode<Cell>(GetPosition(), grid);
	}

	GoToNode(startNode, endNode, grid);
}

void Agent::GoToNode(Node<Cell>* startNode, Node<Cell>* endNode, Grid* grid)
{
	SetStartNode(startNode);
	SetEndNode(endNode);

	mPath.Find(startNode, endNode, grid, this);
}

void Agent::ResetPath(Node<Cell>* endNode, Grid* grid)
{
	if (GetTarget().isSet)
	{
		mNeedToWaitForReset = true;
		return;
	}

	mNeedToWaitForReset = false;

	if (GetRoam())
	{
		//recalculer le path pour une compatibilit  avec roam
		mPath.SetPath(std::vector<Node<Cell>*>());
		mIndexPath = 0;

		Node<Cell>* startNode = GetStartNode();
		GoToNode(startNode, endNode, grid);

		Node<Cell>* currentNode = GetNode<Cell>(GetPosition(), grid);
		std::vector<Node<Cell>*> newAllPaths = *mPath.GetPath();
		int newIndexPath = GetCurrentIndexOnPath(currentNode, &newAllPaths);

		if (newIndexPath != -1)
		{
			//si le nouveau path passe par la case actuelle
			mIndexPath = newIndexPath + 1;
		}
		else
		{
			//si le nouveau path passe pas par la case actuelle
			mPath.SetPath(std::vector<Node<Cell>*>());

			Node<Cell>* newStartNode = currentNode;
			Node<Cell>* newEndNode = startNode;
			GoToNode(newStartNode, newEndNode, grid);

			//additionner les 2 paths Actuel->Start + Start->End
			std::vector<Node<Cell>*> newNewAllPaths = *mPath.GetPath();
			AddVector2OnVector1<Node<Cell>*>(&newNewAllPaths, newAllPaths);

			mIndexPath = 0;
			mPath.SetPath(newNewAllPaths);
			SetStartNode(startNode);
			SetEndNode(endNode);
		}
	}
	else
	{
		//refaire le path depuis la position actuelle
		mPath.SetPath(std::vector<Node<Cell>*>());
		mIndexPath = 0;
		GoToNode(endNode, grid);
	}
}

int Agent::GetCurrentIndexOnPath(Node<Cell>* currentNode, std::vector<Node<Cell>*>* newAllPaths)
{
	int lenghtNewAllPaths = newAllPaths->size();
	for (int i = 0; i < lenghtNewAllPaths; i++)
	{
		if ((*newAllPaths)[i] == currentNode)
		{
			return i;
			break;
		}
	}
	return -1;
}

void Agent::SetRoam(bool roam)
{
	mRoam = roam;
}
