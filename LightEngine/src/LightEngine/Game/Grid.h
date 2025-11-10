#pragma once
#include <vector>
#include "Cell.h"
#include "Node.h"

template<typename T>
class Grid
{
	int mCellSize = 100; // default
	std::vector<Node<T>> mAllNodes;
	std::vector<std::vector<Cell>> mAllCells;
public:
	Grid(int cellSize) { mCellSize = cellSize; Start(); }
	void Start();
	void Update();
};

#include "Grid.inl"

