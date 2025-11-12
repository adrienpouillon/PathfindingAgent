#include "Cursor.h"
#include "../GameManager.h"
#include "../Entity.h"
#include "MainScene.h"
#include "Grid.h"
#include "../DummyEntity.h"

#include <iostream>

void Cursor::Start()
{
	pCurrentCell = nullptr;
	pCurrentScene = dynamic_cast<MainScene*>(GameManager::Get()->GetScene());

	if(pCurrentScene != nullptr)
		mGridCellSize = pCurrentScene->GetGrid()->GetCellSize();

	mPos = sf::Vector2f(0, 0);
}
void Cursor::Update()
{
	float dt = GameManager::Get()->GetDeltaTime();
	auto win = GameManager::Get()->GetWindow();
	mPos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));

	HandleInputs();
}

void Cursor::HandleInputs()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) == false)
	{
		mIsPressed = false;
	}

	if (mIsPressed)
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		AddAgent<DummyEntity>(mGridCellSize * 0.25f, sf::Color::Cyan);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		mIsPressed = true;
		SwapCellObstalce();
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		for (Entity* e : GameManager::Get()->GetEntities())
		{
			if (e->IsInside(mPos.x, mPos.y))
			{
				e->Destroy();
			}
		}
	}
}

void Cursor::SwapCellObstalce()
{
	sf::Vector2f fixedPos = { 0, 0 };

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
		nearest->SetObstacle(!nearest->GetObstacle());
	}
}

void Cursor::DisplayCoords()
{
	std::cout << "(" << mPos.x << ", " << mPos.y << ")\n";
}
