#pragma once
#include "../Entity.h"
#include "PathFinding.h"

template<typename T>
class Grid;

enum TagAgent
{
	AGENT,
	CIVIL,
};

template<typename T>
class Agent :public Entity
{
protected:
	PathFinding<T> mPath;
	int mIndexPath;
	bool mRoam;
public:
	Agent() { }

	void OnInitialize() { SetPathfinding(PathFinding<T>()); mIndexPath = 0; SetRoam(false); SetPosition(0.f, 0.f); SetTag(AGENT); SetSpeed(1.f); SetRigidBody(false); }

	virtual void OnUpdate();

	virtual void UpdatePath();

	void GoToCell(T* cellEnd, Grid<T>* grid);

	void GoToCell(sf::Vector2f pos, Grid<T>* grid);

	void GoToNode(Node<T>* nodeEnd, Grid<T>* grid);

	void SetPathfinding(PathFinding<T> path) { mPath = path; }
	PathFinding<T>* GetPathfinding() { return &mPath; }

	void SetRoam(bool roam) { mRoam = roam; }
	bool GetRoam() { return mRoam; }
};

#include "Agent.inl"
