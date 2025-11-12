#pragma once

#include <SFML/System/Vector2.hpp>

template<typename T>
class Grid;

template<typename T>
class Node;

namespace Utils
{
	bool Normalize(sf::Vector2f& vector);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);

	int DisManhattan(sf::Vector2f currentPos, sf::Vector2f endPos);
	sf::Vector2f GetCellInPos(sf::Vector2f pos, int sizeCell);

	template<typename T>
	T* GetCell(sf::Vector2f pos, Grid<T>* grid);

	template<typename T>
	Node<T>* GetNode(sf::Vector2f pos, Grid<T>* grid);
}


#include "Utils.inl"
