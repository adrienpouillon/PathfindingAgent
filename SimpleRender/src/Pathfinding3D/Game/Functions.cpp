#include "pch.h"
#include "Functions.h"
#include "../LightEngine/Entity.h"
#include "../LightEngine/GameManager.h"

Entity* GetNearestEntity(gce::Vector3f32 pos)
{
    for (Entity* e : GameManager::Get()->GetEntities())
    {
        if (e->IsInside(pos.x, pos.y, pos.z))
        {
            return e;
        }
    }

    return nullptr;
}

Entity* GetNearestAgent(gce::Vector3f32 pos)
{
    for (Entity* e : GameManager::Get()->GetEntities())
    {
        if (e->IsType(Tag::AGENT) == false)
            continue;

        if (e->IsInside(pos.x, pos.y, pos.z))
        {
            return e;
        }
    }

    return nullptr;
}
