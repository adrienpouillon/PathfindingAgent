#pragma once
#include "../Scene.h"
#include "Cell.h"

#include "Agent.h"

class CGrid;

class Entity;

class MainScene : public Scene
{
protected:
	int mGridRows = 9; // default
	int mGridCols = 9; // default

	CGrid* mpGrid = nullptr;

	sf::View mView;

	Entity* mSelectedEntity = nullptr;

	bool mIsSaving = false;
	bool mIsResizing = false;
public:
	void Clear();

	void CleanEntities();

	void SetGridDimensions(int rows, int cols) { mGridRows = rows; mGridCols = cols; }
	const int& GetGridRows() { return mGridRows; }
	const int& GetGridCols() { return mGridCols; }

	void SetGridRows(int val) { mGridRows = val; }
	void SetGridCols(int val) { mGridCols = val; }

	sf::View& GetView() { return mView; }

	Entity* GetSelectedEntity() { return mSelectedEntity; }
	void SetSelectedEntity(Entity* e) { mSelectedEntity = e; }
	void SetCellObstacle(sf::Vector2f pos, bool state);
	// RIP Cursor 2025-2025
	void InputManager(sf::Vector2f worldMousePos, const sf::Event& e);

	// TMP
	Entity* GetNearestEntity(sf::Vector2f pos);
	Cell* GetNearestCell(sf::Vector2f pos);

	CGrid* GetGrid() { return mpGrid; }

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
	
	void ZoomManager();

	Agent<Cell>* CreateAgent(sf::Vector2f pos, float speed, int radius, sf::Color color);
	PathFinding<Cell> CreatePathFinding();

	void HandleGridSave();
	void HandleGridResizing();
};