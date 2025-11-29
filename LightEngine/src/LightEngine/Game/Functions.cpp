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

int GetRandomNumber(int min, int max)
{
    int range = max - min + 1;
    int value = rand() % range + min;
    return value;
}
