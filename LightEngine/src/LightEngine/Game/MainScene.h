#pragma once
#include "../Scene.h"
#include "PathFinding.h"
#include "Cell.h"

#define DRAW_DEBUG_AGENT_NODE 0
#define DRAW_DEBUG_AGENT DRAW_DEBUG_AGENT_NODE + 1
#define DRAW_DEBUG_NO DRAW_DEBUG_AGENT + 1

#define RADIUS 25.f


constexpr int CamSpeed = 250;

class Agent;
class Civil;
class Guard;
class Assassin;

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

	bool mRestartNode = false;
	bool mIsChangingGridConfig = false;

	int mDrawDebug;
	bool mUseCoin;
public:

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

	// RIP Cursor 2025-2025

	void MousePressedInputs(const sf::Event& event, sf::Vector2f worldMousePos, int& actionExecute);
	void MouseReleasedInputs(const sf::Event& event, sf::Vector2f worldMousePos, int& actionExecute);
	void KeyPressedInputs(const sf::Event& event, sf::Vector2f worldMousePos, int& actionExecute);
	void KeyReleasedInputs(const sf::Event& event, sf::Vector2f worldMousePos, int& actionExecute);
	void MouseInputs(const sf::Event& event, sf::Vector2f worldMousePos, int& actionExecute);
	void KeyInputs(const sf::Event& event, sf::Vector2f worldMousePos, int& actionExecute);

	void HandleCameraTransformations();
	void HandleCamZoom();
	void HandleCamMove();

	void HandleGridSave();
	void HandleChangingGridConfig();

	Agent* CreateAgent(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor);
	Civil* CreateCivil(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor);
	Guard* CreateGuard(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor);
	Assassin* CreateAssassin(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor);

	PathFinding<Cell> CreatePathFinding();

	void Clear();
	void CleanEntities();
	void CleanWalls();

	void ZoomManager();
	void DisplayUI();

	void SwitchGridFile(int val);

	void DrawSelectedEntity();
	void DebugDrawPath();

	void ResetNode();

	sf::View& GetView() { return mView; }

	void SetCellObstacle(sf::Vector2f pos, bool state);
	void SetCellCoin(sf::Vector2f pos, bool state);
	void SetCellObstacleCoin(sf::Vector2f pos, bool stateObstacle, bool stateCoin);

	void SetUseCoin(bool useCoin) { mUseCoin = useCoin; }
	const bool& GetUseCoin() const { return mUseCoin; }

	void SetRestartNode(bool restartNode) { restartNode = restartNode; }
	const bool& GetRestartNode() const { return mRestartNode; }

	Entity* GetSelectedEntity() { return mSelectedEntity; }
	void SetSelectedEntity(Entity* e) { mSelectedEntity = e; }

	Grid* GetGrid() { return mpGrid; }

	void SetGridRows(int val) { mGridRows = val; }
	void SetGridCols(int val) { mGridCols = val; }

	void SetGridDimensions(int rows, int cols) { mGridRows = rows; mGridCols = cols; }

	void SetDrawDebug(int drawDebug) { mDrawDebug = drawDebug; }
	const int& GetDrawDebug() const { return mDrawDebug; }

	const int& GetGridRows() const { return mGridRows; }
	const int& GetGridCols() const { return mGridCols; }
};


