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

	virtual void OnUpdate(float deltatime);

	virtual void UpdatePath();

	void GoToCell(T* cellEnd, std::vector<Node<T>*> allNodes);

	void GoToCell(sf::Vector2f pos, std::vector<Node<T>*> allNodes);

	void GoToNode(Node<T>* nodeEnd);

	T* GetCurrentCell(Grid<T>* grid);
	Node<T>* GetCurrentNode(Grid<T>* grid);

	void SetPathfinding(PathFinding<T> path) { mPath = path; }
	PathFinding<T>* GetPathfinding() { return &mPath; }

	void SetRoam(bool roam) { mRoam = roam; }
	bool GetRoam() { return mRoam; }
};

#include "Agent.inl"
