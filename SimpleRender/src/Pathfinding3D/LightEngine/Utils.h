#pragma once
#include "pch.h"

namespace Utils
{
	bool Normalize(gce::Vector3f32& vector);
	float GetDistance(int x1, int y1, int z1, int x2, int y2, int z2);
	float DisManhattan(gce::Vector3f32 currentPos, gce::Vector3f32 endPos);
	gce::Vector2f32 GetCellInPos(gce::Vector3f32 pos, int sizeCell);
}
