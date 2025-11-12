#pragma once
#include "Entity.h"

#include "GameManager.h"

template<typename T>
T* Entity::GetScene() const
{
	T* pScene = dynamic_cast<T*>(GetScene());

	_ASSERT(pScene != nullptr);

	return pScene;
}

template<typename T>
T* Entity::CreateCircleEntity(float radius, const sf::Color& color)
{
	return GetScene()->CreateCircleEntity<T>(radius, color);
}

template<typename T>
T* Entity::CreateRectEntity(float width, float height, const sf::Color& color)
{
	return GetScene()->CreateRectEntity<T>(width, height, color);
}