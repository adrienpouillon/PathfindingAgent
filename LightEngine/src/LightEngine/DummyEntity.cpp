#include "DummyEntity.h"

#include <iostream>

void DummyEntity::OnInitialize()
{
}

void DummyEntity::OnUpdate()
{
	sf::Vector2f pos = GetPosition();
	SetPosition(pos.x + 20 * GameManager::Get()->GetDeltaTime(), pos.y + 20 * GameManager::Get()->GetDeltaTime());
}

void DummyEntity::OnCollision(Entity* other)
{
}