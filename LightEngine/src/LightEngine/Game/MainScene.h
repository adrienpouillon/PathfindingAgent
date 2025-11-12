#pragma once
#include "../Scene.h"
#include "Cell.h"

template<typename T>
class Grid;

class Cursor;

class MainScene : public Scene
{
	int mGridRows = 9; // default
	int mGridCols = 9; // default
	Cursor* mpCursor = nullptr;

	Grid<Cell>* mpGrid = nullptr;

	sf::View mView;
public:
	void Clear();

	void SetGridSize(int rows, int cols) { mGridRows = rows; mGridCols = cols; }
	const int& GetGridRows() { return mGridRows; }
	const int& GetGridCols() { return mGridCols; }

	sf::View& GetView() { return mView; }

	Grid<Cell>* GetGrid() { return mpGrid; }

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

