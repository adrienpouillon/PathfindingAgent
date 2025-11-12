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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Agent<Cell>* e = AddAgent<Agent<Cell>>(mGridCellSize * 0.25f, sf::Color::Cyan);
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

void Cursor::DisplayCoords()
{
	std::cout << "(" << mPos.x << ", " << mPos.y << ")\n";
}
