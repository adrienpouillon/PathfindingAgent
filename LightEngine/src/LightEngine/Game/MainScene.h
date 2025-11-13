#pragma once
#include "../Scene.h"
#include "PathFinding.h"
#include "Cell.h"

class Agent;

class Grid;

class Entity;

class MainScene : public Scene
{
protected:
	int mGridRows = 9; // default
	int mGridCols = 9; // default

	Grid* mpGrid = nullptr;

	sf::View mView;

	Entity* mSelectedEntity = nullptr;

	bool mIsSaving = false;
	bool mIsResizing = false;
public:

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

	// RIP Cursor 2025-2025
	void InputManager(sf::Vector2f worldMousePos, const sf::Event& e);

	void HandleGridSave();
	void HandleGridResizing();

	Agent* CreateAgent(sf::Vector2f pos, float speed, int radius, sf::Color color);
	PathFinding<Cell> CreatePathFinding();

	void Clear();
	void CleanEntities();

	void ZoomManager();

	sf::View& GetView() { return mView; }

	void SetCellObstacle(sf::Vector2f pos, bool state);

	Entity* GetSelectedEntity() { return mSelectedEntity; }
	void SetSelectedEntity(Entity* e) { mSelectedEntity = e; }

	Grid* GetGrid() { return mpGrid; }

	void SetGridRows(int val) { mGridRows = val; }
	void SetGridCols(int val) { mGridCols = val; }

	void SetGridDimensions(int rows, int cols) { mGridRows = rows; mGridCols = cols; }
	const int& GetGridRows() { return mGridRows; }
	const int& GetGridCols() { return mGridCols; }
};