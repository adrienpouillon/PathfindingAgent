#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "Node.h"

class MainScene;

class CGrid
{
	int mCellSize;
	std::vector<Node<Cell>*> mAllNodes;
	std::vector<std::vector<Cell*>> mAllCells;

	MainScene* pCurrentScene = nullptr;
public:
	CGrid(int cellSize) { mCellSize = cellSize; Start(); }

	void Start();
	void InitTab(std::string strGrid);
	void CreateTab(int rows, int cols, std::string strGrid);
	void InitNodeNeighbor(int rows, int cols);

	void CleanGrid();
	
	void SaveGrid(std::string fileName);
	std::string GetStringFromTxt(std::string fileName);

	void Update();
	void UpdateCellsStatut();

	void DrawGrid();
	void DrawColorCell();
	void DrawLineGrid();

	sf::Vector2f GetPositionToView(int rows, int cols, float coef, int size);

	const int& GetCellSize() { return mCellSize; }
	void SetCellSize(int val) { mCellSize = val; }

	std::vector<std::vector<Cell*>>& GetAllCells() { return mAllCells; }

	std::vector<Node<Cell>*>& GetAllNodes() { return mAllNodes; }
};