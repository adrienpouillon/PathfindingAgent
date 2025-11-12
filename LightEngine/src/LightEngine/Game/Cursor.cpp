#include "Cursor.h"
#include "../GameManager.h"
#include "../Entity.h"
#include "MainScene.h"
#include "Grid.h"
#include "Agent.h"

#include <iostream>

void Cursor::Start()
{
	pCurrentCell = nullptr;
	pCurrentScene = GameManager::Get()->GetScene<MainScene>();

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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Agent<Cell>* e = AddAgent<Agent<Cell>>(mGridCellSize * 0.25f, sf::Color::Cyan);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		SetCellObstalce(true);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		for (Entity* e : GameManager::Get()->GetEntities())
		{
			if (e->IsInside(mPos.x, mPos.y))
			{
				e->Destroy();
				return;
			}
		}

		SetCellObstalce(false);
	}
}

void Cursor::SetCellObstalce(bool state)
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
		if (nearest->GetAgent() == false)
		{
			nearest->SetObstacle(state);
		}
	}
}

void Cursor::DisplayCoords()
{
	std::cout << "(" << mPos.x << ", " << mPos.y << ")\n";
}
