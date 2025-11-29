#include "Cell.h"
#include "../GameManager.h"
#include "../Entity.h"
#include "../Debug.h"
#include <iostream>
#include "Agent.h"

void Cell::Update()
{
}

void Cell::CheckStatus(int cellSize)
{
	Collide(cellSize);
	if (HasAgent() && HasCoin())
	{
		GetAgent()->IncreaseCoin(1);
		SetHasCoin(false);
	}
}

void Cell::Collide(int cellSize)
{
	for (Entity* e : GameManager::Get()->GetEntities())
	{
		sf::Vector2f pos = getPosition();
		sf::Vector2f ePos = e->GetPosition();

		sf::Vector2f entitySize = e->GetSize();

		if (ePos.x + entitySize.x * 0.5f > pos.x - cellSize * 0.5f && ePos.x - entitySize.x * 0.5f < pos.x + cellSize * 0.5f)
		{
			if (ePos.y + entitySize.y * 0.5f > pos.y - cellSize * 0.5f && ePos.y - entitySize.y * 0.5f < pos.y + cellSize * 0.5f)
			{
				if (Agent* a = dynamic_cast<Agent*>(e))
				{
					SetAgent(a);
				}
				else
				{
					SetAgent(nullptr);
				}

				SetHasAgent(true);
				return;
			}
		}
	}

	SetAgent(nullptr);
	SetHasAgent(false);
}
