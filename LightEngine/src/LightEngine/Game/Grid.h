#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "Node.h"

class MainScene;

template<typename T>
class Grid
{
	int mCellSize = 100; // default
	std::vector<Node<T>> mAllNodes;
	std::vector<std::vector<Cell>> mAllCells;

	MainScene* pCurrentScene = nullptr;
public:
	Grid(int cellSize) { mCellSize = cellSize; Start(); }
	void Start();
	void EraseGrid();
	void SaveGrid(std::string fileName);
	void InitGridFromTxt(std::string fileName);
	void Update();
	void DrawGrid();
	void UpdateCellsStatut();
	const int& GetCellSize() { return mCellSize; }
	std::vector<std::vector<Cell>>& GetAllCells() { return mAllCells; }
};

#include "Grid.inl"

