#include "Cell.h"
#include "../GameManager.h"
#include "../Entity.h"

void Cell::CheckStatus()
{
	for (Entity* e : GameManager::Get()->GetEntities())
	{
		sf::Vector2f pos = getPosition();
		sf::Vector2f ePos = e->GetPosition();
		
		if (ePos.x - e->GetSize().x * 0.5f >= pos.x - mSize && ePos.x + e->GetSize().x * 0.5f <= pos.x + mSize)
		{
			if (ePos.y - e->GetSize().y * 0.5f >= pos.y - mSize && ePos.y + e->GetSize().y * 0.5f <= pos.y + mSize)
			{
				SetAgent(true);
				return;
			}
		}
	}

	SetAgent(false);
}
