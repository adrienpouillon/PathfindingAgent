#include "Cursor.h"
#include "../GameManager.h"
#include "../Scene.h"
#include "../Entity.h"
#include "../DummyEntity.h"

#include <iostream>

void Cursor::Start()
{
	pCurrentCell = nullptr;
	pCurrentScene = GameManager::Get()->GetScene();
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
		AddAgent();
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		for (Entity* e : GameManager::Get()->GetEntities())
		{
			if (e->IsInside(mPos.x, mPos.y))
			{
				e->Destroy();
				break;
			}
		}
	}
}

void Cursor::AddAgent(Entity* pInstance)
{
	Entity* newEntity = pCurrentScene->CreateEntity<DummyEntity>(10, sf::Color::Cyan);

	newEntity->SetPosition(mPos.x, mPos.y);
}

void Cursor::DisplayCoords()
{
	std::cout << "(" << mPos.x << ", " << mPos.y << ")\n";
}
