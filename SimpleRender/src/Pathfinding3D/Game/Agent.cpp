#include "pch.h"
#include "Agent.h"

#include "../LightEngine/Debug.h"
#include "../LightEngine/GameManager.h"
#include "Node.h"
#include "Grid.h"
#include "MainScene.h"
#include "Functions.h"
#include <iostream>

void Agent::OnInitialize()
{
	SetTag(AGENT);

	SetPathfinding(PathFinding<Cell>());
	mIndexPath = 0;
	mConfigHeight = -1;
	SetRoam(false);
	SetStartNode(nullptr);
	SetEndNode(nullptr);
	SetSpeed(1.f);
	SetClassicColor(gce::Vector3f32(1.f, 1.f, 1.f));
	SetRoamColor(gce::Vector3f32(0.f, 0.f, 0.f));
	GetGeo()->SetDefaultColor(mClassicColor);
	int coin = GetRandomNumber(0, 10);
	if (coin == 10)
	{
		coin = GetRandomNumber(0, 18);
	}
	SetCoin(coin);

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
	RecreatCoin();

	if (mpCurrentScene->GetSelectedEntity() == this)
	{
		mpCurrentScene->SetSelectedEntity(nullptr);
	}
}

void Agent::RecreatCoin()
{
	gce::Vector3f32 pos = GetPosition();
	std::vector<std::vector<Cell*>> allCells = mpCurrentScene->GetGrid()->GetAllCells();
	int size = mpCurrentScene->GetGrid()->GetCellSize();

	int i = 1;
	int createCoin = 0;
	bool round = true;
	while (round)
	{
		Cell* nearest;
		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(i * size,0.f, 0.f), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }
		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(0.f, 0.f, i * size), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }
		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(-i * size, 0.f, 0.f), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }
		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(0.f, 0.f, -i * size), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }

		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(i * size, 0.f, i * size), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }
		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(i * size, 0.f, -i * size), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }
		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(-i * size, 0.f, i * size), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }
		nearest = GetNearestCell<Cell>(pos + gce::Vector3f32(-i * size, 0.f, -i * size), allCells);
		if (nearest->IsObstacle() == false && nearest->HasCoin() == false) { nearest->SetHasCoin(true); createCoin++; }
		if (createCoin > mCoin) { round = false; }

		i++;
	}
}


void Agent::UpdatePath()
{
	if (mpCurrentScene->GetDrawDebug() == DRAW_DEBUG_AGENT)
	{
		for (int i = mIndexPath; i < mPath.GetPath()->size(); i++)
		{
			(*mPath.GetPath())[i]->GetData()->GetGeo()->SetColor(gce::Vector3f32(1.f, 0.f, 0.f));
		}
	}

	std::vector<Node<Cell>*>* allPaths = mPath.GetPath();
	if (allPaths->size() < 1)
	{
		mIndexPath = 0;
		OnPathFinish();
		return;
	}

	Grid* grid = mpCurrentScene->GetGrid();

	if (mpCurrentScene->GetRestartNode() || mNeedToWaitForReset == true)
	{
		Node<Cell>* endNode = GetEndNode();
		ResetPath(endNode, grid);
	}

	int lenghtAllPaths = allPaths->size();
	bool haveGoodLenghtIndexPath = (mIndexPath == lenghtAllPaths);

	if (GetTarget().isSet)
	{
		return;
	}

	if (haveGoodLenghtIndexPath)
	{
		mPath.SetReadFinish(true);
		mIndexPath = 0;

		bool roam = GetRoam();
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

		if (mWaitProgress <= mWaitTime && mIsGhost == false && mIsGoingDown == false) // Attendre
		{
			if (nextCell->HasAgent() || currentIsEmpty || nextCell->IsObstacle())
			{
				mWaitProgress += GetDeltaTime();
				return;
			}
		}

		if (mWaitProgress >= mWaitTime && mIsGhost == false && mIsGoingDown == false) // Si trop attendu
		{
			mNeedToWaitForReset = true;
		}
		else
		{
			gce::Vector3f32 nextCellPos = nextCell->GetPosition();
			gce::Vector3f32 myPos = GetPosition();

			float height = (GetGeo()->GetScale().y + nextCell->GetRealHeight()) * 0.5f;
			int diff = nextCell->GetConfigHeight() - mConfigHeight;

			if (diff > 0)
			{
				GoToPosition(myPos.x, nextCellPos.y + height, myPos.z);
			}
			else if (diff < 0)
			{
				GoToPosition(nextCellPos.x, myPos.y, nextCellPos.z);
				mIsGoingDown = true;
			}
			else if (diff == 0)
			{
				GoToPosition(nextCellPos.x, nextCellPos.y + height, nextCellPos.z);
				mIsGoingDown = false;
				mIndexPath++;
			}

			mConfigHeight = nextCell->GetConfigHeight();
		}

		mWaitProgress = 0.f;
	}
}

void Agent::GoToCell(Cell* cellEnd, Grid* grid)
{
	gce::Vector2i32 indexRowCol = cellEnd->GetRowCol();
	std::vector<Node<Cell>*> allNodes = grid->GetAllNodes();
	int lengthCell = grid->GetCellSize();
	GoToNode(GetNodeInTab((int)indexRowCol.x, (int)indexRowCol.y, lengthCell, &allNodes), grid);
}

void Agent::GoToCell(gce::Vector3f32 pos, Grid* grid)
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
		//position finale du path precedent
		startNode = (*currentPath)[length - 1];
	}
	else
	{
		//position actuelle
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
	MainScene* pScene = GameManager::Get()->GetScene<MainScene>();

	if (GetTarget().isSet)
	{
		mNeedToWaitForReset = true;
		return;
	}

	mNeedToWaitForReset = false;

	if (GetRoam())
	{
		//recalculer le path pour une compatibilite  avec roam
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