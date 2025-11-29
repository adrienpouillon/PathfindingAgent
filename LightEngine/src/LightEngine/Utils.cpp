#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "Utils.h"
#include "Game/Functions.h"
#include "GameManager.h"
#include "Game/MainScene.h"

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

	float DisManhattan(sf::Vector2f currentPos, sf::Vector2f endPos, int sizeCell)
	{
		return (abs(currentPos.x - endPos.x) + abs(currentPos.y - endPos.y)) / sizeCell;
	}

	sf::Vector2f GetCellInPos(sf::Vector2f pos, int sizeCell)
	{
		Cell* nearest = GetNearestCell<Cell>(pos, GameManager::Get()->GetScene<MainScene>()->GetGrid()->GetAllCells());

		if (!nearest)
			return { 0, 0 };

		return 	nearest->GetRowCol();

	}
}