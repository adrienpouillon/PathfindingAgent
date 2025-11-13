#include "Functions.h"
#include "../Entity.h"
#include "../GameManager.h"

Entity* GetNearestEntity(sf::Vector2f pos)
{
    for (Entity* e : GameManager::Get()->GetEntities())
    {
        if (e->IsInside(pos.x, pos.y))
        {
            return e;
        }
    }

    return nullptr;
}
