#include "pch.h"
#include "Cursor.h"
#include "Cell.h"
#include "Functions.h"
#include "MainScene.h"

void Cursor::OnInitialize()
{
	SetTag(Tag::CURSOR);
	SetSpeed(150.f);

	mpCurrentScene = GameManager::Get()->GetScene<MainScene>();
}

void Cursor::OnUpdate()
{
	HandleInputs();

	gce::Vector3f32 pos = GetPosition();

	if (Cell* cell = GetNearestCell(pos, mpCurrentScene->GetGrid()->GetAllCells()))
	{
		GetGeo()->SetPosition({ pos.x, cell->GetPosition().y + GetGeo()->GetScale().y + cell->GetRealHeight() * 0.5f, pos.z });
	}
}

void Cursor::OnCollision(Entity* other)
{
}

void Cursor::HandleInputs()
{
	float dt = GetDeltaTime();

	float finalSpeedDt = CamSpeed * dt;

	gce::Vector2i32 newPos = { 0, 0 };

	if (GetKey(Keyboard::LCTRL)) // Handle the Grid Save
	{
		return;
	}

	int xFactor = GetKeyDown(Keyboard::S) - GetKeyDown(Keyboard::Z);
	int yFactor = GetKeyDown(Keyboard::D) - GetKeyDown(Keyboard::Q);

	if (xFactor == 0 && yFactor == 0)
		return;

	newPos.x = mFollowRows + xFactor;
	newPos.y = mFollowCols + yFactor;

	Cell* currentCell = nullptr;

	for (auto& row : mpCurrentScene->GetGrid()->GetAllCells())
	{
		for (Cell* c : row)
		{
			gce::Vector2i32 rowCol = c->GetRowCol();

			if (rowCol.x == newPos.x && rowCol.y == newPos.y)
			{
				currentCell = c;
				break;
			}
		}
	}

	if (currentCell != nullptr)
	{
		gce::Vector3f32 pos = GetPosition();
		gce::Vector3f32 cellPos = currentCell->GetPosition();

		GetGeo()->SetPosition({cellPos.x, pos.y, cellPos.z});

		mFollowRows = newPos.x;
		mFollowCols = newPos.y;
	}
}