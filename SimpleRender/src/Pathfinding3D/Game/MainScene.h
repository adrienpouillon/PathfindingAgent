#pragma once
#include "../LightEngine/Scene.h"
#include "PathFinding.h"
#include "Cell.h"

#define DRAW_DEBUG_AGENT 0
#define DRAW_DEBUG_NO DRAW_DEBUG_AGENT + 1

constexpr int CamSpeed = 150;

class Entity;
class Agent;
class Civil;
class Guard;
class Assassin;

class Grid;
class Cursor;

class MainScene : public Scene
{
protected:
	int mGridRows = 9; // default
	int mGridCols = 9; // default

	Camera* mpCam;

	// Le retour
	Cursor* mpCursor = nullptr;
	Entity* mSelectedEntity = nullptr;

	Grid* mpGrid = nullptr;

	bool mIsSaving = false;
	bool mRestartNode = false;

	int mDrawDebug = 0;
	bool mUseCoin = false;
public:

	void OnInitialize() override;
	void OnUpdate() override;

	// RIP Cursor 2025-2025
	void InputManager();

	void MousePressedInputs(gce::Vector3f32 cursorPos, int& actionExecute);
	void KeyPressedInputs(gce::Vector3f32 cursorPos, int& actionExecute);
	void MouseInputs(gce::Vector3f32 cursorPos, int& actionExecute);
	void KeyInputs(gce::Vector3f32 cursorPos, int& actionExecute);

	void HandleCameraTransformations();
	void HandleResizingGrid();
	void HandleCamMove();

	void HandleGridSave();
	void HandleChangingGridConfig();

	Cursor* GetCursor() { return mpCursor; }

	Agent* CreateAgent(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor);
	Civil* CreateCivil(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor);
	Guard* CreateGuard(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor);
	Assassin* CreateAssassin(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor);
	PathFinding<Cell> CreatePathFinding();

	void Clear();
	void CleanEntities();
	void CleanWalls();

	void HandleCamZoom();
	void DisplayUI();

	void ResetCamSettings();

	void SwitchGridFile(int val);

	void DrawSelectedEntity();
	void DebugDrawPath();

	void ResetNode();

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
	const int& GetGridRows() const { return mGridRows; }
	const int& GetGridCols() const { return mGridCols; }

	void SetDrawDebug(int drawDebug) { mDrawDebug = drawDebug; }
	const int& GetDrawDebug() const { return mDrawDebug; }
};
