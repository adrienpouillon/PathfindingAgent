#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "Node.h"

class MainScene;

template<typename T>
class Grid
{
	int mCellSize;
	std::vector<Node<T>*> mAllNodes;
	std::vector<std::vector<T*>> mAllCells;

	MainScene* pCurrentScene;
public:
	Grid() {Start(); }

	void Start();
	void InitTab(std::string strGrid);
	void CreateTab(int rows, int cols, std::string strGrid);
	void InitNodeNeighbor(int rows, int cols);

	template<typename A>
	std::vector<A*>* EraseTab(std::vector<A*>* all);
	template<typename A>
	std::vector<std::vector<A*>>* EraseTab(std::vector<std::vector<A*>>* all);

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

	std::vector<std::vector<T*>>& GetAllCells() { return mAllCells; }

	std::vector<Node<T>*>& GetAllNodes() { return mAllNodes; }
};

#include "Grid.inl"

