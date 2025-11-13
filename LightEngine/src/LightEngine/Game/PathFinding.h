#pragma once
#include <queue>
#include "Node.h"

class Grid;

template<typename T>
class PathFinding
{
public://protected:
	bool mReadFinish;
	std::vector<Node<T>*> mPath;
public:
	PathFinding() { Start(); }

	void Start() { SetReadFinish(true); }

	void Find(Node<T>* startNode, Node<T>* endNode, Grid* grid);

	void SetReadFinish(bool readFinish);
	bool GetReadFinish();

	void SetPath(std::vector<Node<T>*> path);
	std::vector<Node<T>*>* GetPath();

	~PathFinding();
};

#include "PathFinding.inl"