#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "Node.h"
#include "GridConfigs.h"

class MainScene;

class Grid
{
protected:
	int mCellSize;
	std::vector<Node<Cell>*> mAllNodes;
	std::vector<std::vector<Cell*>> mAllCells;

	MainScene* pCurrentScene = nullptr;
public:
	Grid() { Start(); }

	void Start();
	void InitTab(std::string fileName);
	void InitTab(int _rows, int _cols, std::string strGrid = "");
	void InitNodeNeighbor();

	void CleanGrid();

	void SaveGrid(std::string fileName);
	std::string GetStringFromTxt(std::string fileName);

	void Update();
	void UpdateCellsStatut();

	void DrawGrid();
	void DrawColorCell();

	gce::Vector3f32 GetPositionToView(int rows, int cols, float coef);

	void SetParameters(int cellSize, bool usingTxt = false);
	const int& GetCellSize() const { return mCellSize; }

	std::vector<std::vector<Cell*>>& GetAllCells() { return mAllCells; }

	std::vector<Node<Cell>*>& GetAllNodes() { return mAllNodes; }

protected:
	void CreateTab(int _rows, int _cols, std::string strGrid);
	void InitNodeNeighbor(int rows, int cols);

};