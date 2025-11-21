#include "pch.h"
#include <cmath>
#include "Utils.h"
#include "Game/Functions.h"
#include "GameManager.h"
#include "Game/MainScene.h"

namespace Utils 
{
    bool Normalize(gce::Vector3f32& vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
			vector.z /= magnitude;
			return true;
		}

		return false;
    }

	float GetDistance(int x1, int y1, int z1, int x2, int y2, int z2)
	{
		int x = x2 - x1;
		int y = y2 - y1;
		int z = z2 - z1;

		return std::sqrt(x * x + y * y + z * z);
	}

	float DisManhattan(gce::Vector3f32 currentPos, gce::Vector3f32 endPos)
	{
		return abs(currentPos.x - endPos.x) + abs(currentPos.z - endPos.z);
	}

	gce::Vector2f32 GetCellInPos(gce::Vector3f32 pos, int sizeCell)
	{
		Cell* nearest = GetNearestCell<Cell>(pos, GameManager::Get()->GetScene<MainScene>()->GetGrid()->GetAllCells());

		if (!nearest)
			return { 0, 0 };

		return 	nearest->GetRowCol();
	}
}