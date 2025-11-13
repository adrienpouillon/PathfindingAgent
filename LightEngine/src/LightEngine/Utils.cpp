#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "Utils.h"
#include "Game/Node.h"
#include "Game/Cell.h"

namespace Utils 
{
    bool Normalize(sf::Vector2f& vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
		
			return true;
		}

		return false;
    }

	float GetDistance(int x1, int y1, int x2, int y2)
	{
		int x = x2 - x1;
		int y = y2 - y1;

		return std::sqrt(x * x + y * y);
	}

	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot) * 180 / 3.14159265;
	}

	int DisManhattan(sf::Vector2f currentPos, sf::Vector2f endPos)
	{
		return abs(currentPos.x - endPos.x) + abs(currentPos.y - endPos.y);
	}

	sf::Vector2f GetCellInPos(sf::Vector2f pos, int sizeCell)
	{
		return sf::Vector2f((int)(pos.x / (float)sizeCell), (int)(pos.y / (float)sizeCell));
	}

	Cell* GetCell(sf::Vector2f pos, CGrid* grid)
	{
		sf::Vector2f index = GetCellInPos(pos, grid->GetCellSize());
		return grid->GetAllCells()[index.x][index.y];
	}

	Node<Cell>* GetNode(sf::Vector2f pos, CGrid* grid)
	{
		sf::Vector2f index = GetCellInPos(pos, grid->GetCellSize());
		std::vector<Node<Cell>*> allNodes = grid->GetAllNodes();
		return Node<Cell>::GetNodeInTab(index.x, index.y, grid->GetAllCells().size(), &allNodes);
	}

}