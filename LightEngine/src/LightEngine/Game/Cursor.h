#pragma once
#include <SFML/Graphics.hpp>
#include "MainScene.h"
#include <cmath>

class Entity;

class Cell;

class Cursor
{
	sf::Vector2f mPos;

	Cell* pCurrentCell;
	MainScene* pCurrentScene;

	int mGridCellSize = 1;

	bool mIsPressed = false;
public:
	Cursor() { Start(); }

	void Start();
	void Update();

	//Get
	const sf::Vector2f& GetPosition() { return mPos; }

	void HandleInputs();

	template<typename T>
	void AddAgent(int radius, sf::Color color);
	void SwapCellObstalce();
	void DisplayCoords();
};

template<typename T>
inline void Cursor::AddAgent(int radius, sf::Color color)
{
	sf::Vector2f fixedPos = {0, 0};

	Cell* nearest = nullptr;
	float smallestSquaredDist = INT_MAX;

	for (auto& row : pCurrentScene->GetGrid()->GetAllCells())
	{
		for (auto& cell : row)
		{
			float dx = abs(cell.getPosition().x - mPos.x);
			float dy = abs(cell.getPosition().y - mPos.y);

			float squaredDist = dx * dx + dy * dy;

			if (squaredDist < smallestSquaredDist)
			{
				nearest = &cell;

				smallestSquaredDist = squaredDist;
			}
		}
	}

	if (nearest != nullptr)
	{
		if (nearest->GetAgent() == true)
			return;

		Entity* newEntity = pCurrentScene->template CreateCircleEntity<T>(radius, color);

		fixedPos = nearest->getPosition();

		newEntity->SetPosition(fixedPos.x, fixedPos.y);
	}
}
