#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class MainScene;
class Entity;
class Cell;

class Cursor
{
	sf::Vector2f mPos;

	MainScene* pCurrentScene;

	Entity* mEntitySelected;

	int mGridCellSize = 1;

public:
	Cursor() { Start(); }

	void Start();
	void Update();

	//Get
	const sf::Vector2f& GetPosition() { return mPos; }

	void HandleInputs();

	void* NearestCell();
	Entity* IsInsideEntity();

	void SetCellObstalce(bool state);
	void DisplayCoords();

	void SetEntitySelected(Entity* entitySelected) { mEntitySelected = entitySelected; }
	Entity* GetEntitySelected() { return mEntitySelected; }
};
