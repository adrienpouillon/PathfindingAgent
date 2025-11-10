#pragma once
#include <SFML/Graphics.hpp>

class Entity;

class Cell;
class Scene;

class Cursor
{
	sf::Vector2f mPos;

	Cell* pCurrentCell;
	Scene* pCurrentScene;
public:
	Cursor() { Start(); }

	void Start();
	void Update();

	//Get
	const sf::Vector2f& GetPosition() { return mPos; }

	void HandleInputs();

	void AddAgent(Entity* pInstance = nullptr);

	void DisplayCoords();
};
