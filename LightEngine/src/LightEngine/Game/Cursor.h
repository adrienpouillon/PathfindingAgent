#pragma once
#include <SFML/Graphics.hpp>

class Cell;

class Cursor
{
	sf::Vector2f mPos;

	Cell* pCurrentCell;
public:
	Cursor() { Start(); }

	void Start() { pCurrentCell = nullptr; mPos = sf::Vector2f(0, 0); }
	void Update();

	void HandleKey()
	{
		
	}

	void DisplayCoords();
};
