#pragma once
#include "../Scene.h"
#include "Cell.h"

#include "Agent.h"

template<typename T>
class Grid;

class Cursor;


class MainScene : public Scene
{
protected:
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

	void SetGridRows(int val) { mGridRows = val; }
	void SetGridCols(int val) { mGridCols = val; }

	sf::View& GetView() { return mView; }

	Grid<Cell>* GetGrid() { return mpGrid; }

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

	Agent<Cell>* CreateAgent(sf::Vector2f pos, float speed, int radius, sf::Color color);
	PathFinding<Cell> CreatePathFinding();
	
};