#include "Cell.h"
#include "../GameManager.h"
#include "../Entity.h"
#include "../Debug.h"
#include <iostream>

void Cell::Update()
{
}

void Cell::CheckStatus(int size)
{
	for (Entity* e : GameManager::Get()->GetEntities())
	{
		sf::Vector2f pos = getPosition();
		sf::Vector2f ePos = e->GetPosition();
		
		if (ePos.x - e->GetSize().x * 0.5f >= pos.x - size && ePos.x + e->GetSize().x * 0.5f <= pos.x + size)
		{
			if (ePos.y - e->GetSize().y * 0.5f >= pos.y - size && ePos.y + e->GetSize().y * 0.5f <= pos.y + size)
			{
				SetAgent(true);
				return;
			}
		}
	}

	SetAgent(false);
}
