#pragma once
#include "Scene.h"
#include "GameManager.h"
#include "Entity.h"

template<typename T>
T* Scene::CreateCircleEntity(float radius, const sf::Color& color)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	T* newEntity = new T();

	Entity* entity = newEntity;
	entity->SetShapeTag((int)ShapeType::Circle);
	sf::CircleShape* pShape = entity->CreateShape<sf::CircleShape>();
	pShape->setRadius(radius);
	entity->Initialize(color);
	
	mpGameManager->mEntitiesToAdd.push_back(newEntity);

	return newEntity;
}

template<typename T>
T* Scene::CreateRectEntity(float width, float height, const sf::Color& color)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	T* newEntity = new T();

	Entity* entity = newEntity;
	entity->SetShapeTag((int)ShapeType::Rectangle);
	sf::RectangleShape* pShape = entity->CreateShape<sf::RectangleShape>();
	pShape->setSize(sf::Vector2f(width, height));
	entity->Initialize(color);

	mpGameManager->mEntitiesToAdd.push_back(newEntity);

	return newEntity;
}
