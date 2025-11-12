#include "DummyEntity.h"

#include <iostream>

void DummyEntity::OnInitialize()
{
}

void DummyEntity::OnUpdate()
{
	sf::Vector2f pos = GetPosition();

	sf::Vector2f direction = { 0, 0 };

	direction.x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
	direction.y = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::Z);

	SetDirection(direction.x, direction.y, 100);
}

void DummyEntity::OnCollision(Entity* other)
{
}