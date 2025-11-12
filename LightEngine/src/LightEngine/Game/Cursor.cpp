#include "Cursor.h"
#include "../GameManager.h"
#include "../Entity.h"
#include "MainScene.h"
#include "Grid.h"
#include "Agent.h"
#include "../Debug.h"

#include <iostream>

void Cursor::Start()
{
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

	if (mEntitySelected != nullptr)
	{
		sf::Vector2f pos = mEntitySelected->GetPosition();
		Debug::DrawCircle(pos.x, pos.y, 5.f, sf::Color::Magenta);
	}
}

void Cursor::HandleInputs()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Entity* clicOnEntity = IsInsideEntity();
		if(mEntitySelected == nullptr)
		{
			if (clicOnEntity != nullptr)
			{
				//selectionner une entity
				mEntitySelected = clicOnEntity;
			}
			else
			{
				//creer une entity
				sf::Vector2f pos = static_cast<Cell*>(NearestCell())->getPosition();

				pCurrentScene->CreateAgent(pos, 100.f, mGridCellSize * 0.25f, sf::Color::Cyan);
			}
		}
		else
		{
			if (clicOnEntity == nullptr)
			{
				//deplacer une entity
				if (Agent<Cell>* a = dynamic_cast<Agent<Cell>*>(mEntitySelected))
				{
					a->GoToCell(mPos, pCurrentScene->GetGrid());
				}
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		SetCellObstalce(true);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if(Entity* e = IsInsideEntity())
		{
			mEntitySelected = nullptr;
			e->Destroy();
			return;
		}

		SetCellObstalce(false);
	}
}

void* Cursor::NearestCell()
{
	Cell* nearest = nullptr;
	float smallestSquaredDist = INT_MAX;

	std::vector<std::vector<Cell*>> allCells = pCurrentScene->GetGrid()->GetAllCells();
	for (auto row : allCells)
	{
		for (auto cell : row)
		{
			float dx = abs(cell->getPosition().x - mPos.x);
			float dy = abs(cell->getPosition().y - mPos.y);

			float squaredDist = dx * dx + dy * dy;

			if (squaredDist < smallestSquaredDist)
			{
				nearest = cell;

				smallestSquaredDist = squaredDist;
			}
		}
	}
	return nearest;
}

Entity* Cursor::IsInsideEntity()
{
	for (Entity* e : GameManager::Get()->GetEntities())
	{
		if (e->IsInside(mPos.x, mPos.y))
		{
			return e;
		}
	}
	return nullptr;
}

void Cursor::SetCellObstalce(bool state)
{
	sf::Vector2f fixedPos = { 0, 0 };

	Cell* nearest = static_cast<Cell*>(NearestCell());

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
