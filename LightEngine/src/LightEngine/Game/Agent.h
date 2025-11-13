#pragma once
#include "../Entity.h"
#include "PathFinding.h"

class Grid;

enum TagAgent
{
	AGENT,
	CIVIL,
};

class Agent :public Entity
{
protected:
	PathFinding<Cell> mPath;
	int mIndexPath;
	bool mRoam;
	Node<Cell>* mStartNode;
	Node<Cell>* mEndNode;
public:
	Agent() { }

	void OnInitialize() { SetPathfinding(PathFinding<Cell>()); mIndexPath = 0; SetRoam(false); SetStartNode(nullptr); SetEndNode(nullptr); SetPosition(0.f, 0.f); SetTag(AGENT); SetSpeed(1.f); SetRigidBody(false); }

	virtual void OnUpdate();

	virtual void UpdatePath();

	void GoToCell(Cell* cellEnd, Grid* grid);

	void GoToCell(sf::Vector2f pos, Grid* grid);

	void GoToNode(Node<Cell>* nodeEnd, Grid* grid);

	void SetPathfinding(PathFinding<Cell> path) { mPath = path; }
	PathFinding<Cell>* GetPathfinding() { return &mPath; }

	void SetStartNode(Node<Cell>* startNode) { mStartNode = startNode; }
	Node<Cell>* GetStartNode() { return mStartNode; }

	void SetEndNode(Node<Cell>* endNode) { mEndNode = endNode; }
	Node<Cell>* GetEndNode() { return mEndNode; }

	void SetRoam(bool roam) { mRoam = roam; }
	bool GetRoam() { return mRoam; }
};

#include "Agent.inl"
