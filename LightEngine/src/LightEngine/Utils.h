#pragma once

#include <SFML/System/Vector2.hpp>

class Grid;

class Cell;

template<typename T>
class Node;

namespace Utils
{
	bool Normalize(sf::Vector2f& vector);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);

	int DisManhattan(sf::Vector2f currentPos, sf::Vector2f endPos);
	sf::Vector2f GetCellInPos(sf::Vector2f pos, int sizeCell);

	Cell* GetCell(sf::Vector2f pos, Grid* grid);

	Node<Cell>* GetNode(sf::Vector2f pos, Grid* grid);
}


#include "Utils.inl"
