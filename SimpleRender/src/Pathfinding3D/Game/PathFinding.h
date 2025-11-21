#pragma once
#include <queue>
#include "Node.h"

class Grid;

template<typename T>
class PathFinding
{
public:
	bool mReadFinish;
	std::vector<Node<T>*> mPath;
public:
	PathFinding() { Start(); }

	void Start() { SetReadFinish(true); }

	void Find(Node<T>* startNode, Node<T>* endNode, Grid* grid, Agent* pOwner);

	void SetReadFinish(bool readFinish);
	bool GetReadFinish();

	void SetPath(std::vector<Node<T>*> path);
	std::vector<Node<T>*>* GetPath();

	~PathFinding();
};

#include "PathFinding.inl"