#pragma once
#include "../LightEngine/Entity.h"
#include "PathFinding.h"


class MainScene;
class Grid;
class MainScene;

class Agent :public Entity
{
protected:
	PathFinding<Cell> mPath;
	int mIndexPath;
	bool mRoam;
	Node<Cell>* mStartNode;
	Node<Cell>* mEndNode;

	gce::Vector3f32 mClassicColor;
	gce::Vector3f32 mRoamColor;

	MainScene* mpCurrentScene;

	bool mNeedToWaitForReset = false;

	float mWaitTime = 1.f;
	float mWaitProgress = 0.f;

	int mConfigHeight;

	bool mIsGhost = false;
	bool mIsGoingDown = false;

	int mCoin;
public:
	virtual void OnInitialize();

	virtual void OnUpdate();
	virtual void OnPathFinish();
	virtual void OnDestroy();

	void RecreatCoin();

	virtual void UpdatePath();

	void GoToCell(Cell* cellEnd, Grid* grid);
	void GoToCell(gce::Vector3f32 pos, Grid* grid);

	void GoToNode(Node<Cell>* nodeEnd, Grid* grid);
	void GoToNode(Node<Cell>* startNode, Node<Cell>* endNode, Grid* grid);

	void ResetPath(Node<Cell>* endNode, Grid* grid);

	int GetCurrentIndexOnPath(Node<Cell>* currentNode, std::vector<Node<Cell>*>* newAllPaths);

	void SetPathfinding(PathFinding<Cell> path) { mPath = path; }
	PathFinding<Cell>* GetPathfinding() { return &mPath; }

	void SetStartNode(Node<Cell>* startNode) { mStartNode = startNode; }
	Node<Cell>* GetStartNode() { return mStartNode; }

	void SetEndNode(Node<Cell>* endNode) { mEndNode = endNode; }
	Node<Cell>* GetEndNode() { return mEndNode; }

	const int& GetConfigHeight() const { return mConfigHeight; }
	void SetConfigHeight(int height) { mConfigHeight = height; }

	void SetRoam(bool roam)
	{
		mRoam = roam;
		if (roam)
		{
			GetGeo()->SetDefaultColor(mRoamColor);
		}
		else
		{
			GetGeo()->SetDefaultColor(mClassicColor);
		}
	}

	const bool& GetRoam() const { return mRoam; }

	void SetGhost(bool state) { mIsGhost = state; }
	const bool& IsGhost() const { return mIsGhost; }

	void SetClassicColor(gce::Vector3f32 color) { mClassicColor = color; GetGeo()->SetDefaultColor(mClassicColor); }
	const gce::Vector3f32& GetClassicColor() const { return mClassicColor; }

	void SetRoamColor(gce::Vector3f32 color) { mRoamColor = color; }
	const gce::Vector3f32& GetRoamColor() const { return mRoamColor; }

	void SetCurrentScene(MainScene* currentScene) { mpCurrentScene = currentScene; }
	MainScene* GetCurrentScene() { return mpCurrentScene; }

	void SetCoin(int coin) { mCoin = coin; }
	void IncreaseCoin(int add) { mCoin += add; }
	const int& IsCoin() const { return mCoin; }
};
