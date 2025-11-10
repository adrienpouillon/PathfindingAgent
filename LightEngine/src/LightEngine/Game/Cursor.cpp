#include "Cursor.h"
#include "../GameManager.h"
#include <iostream>

void Cursor::Update()
{
	float dt = GameManager::Get()->GetDeltaTime();

	auto win = GameManager::Get()->GetWindow();

	mPos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
}

void Cursor::DisplayCoords()
{
	std::cout << "(" << mPos.x << ", " << mPos.y << ")\n";
}
