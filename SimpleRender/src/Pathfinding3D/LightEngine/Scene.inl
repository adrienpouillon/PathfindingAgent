#pragma once
#include "Scene.h"
#include "GameManager.h"
#include "Entity.h"

template<typename T>
T* Scene::CreateEntity(Geometry* pGeo, gce::Vector3f32 scale, const gce::Vector3f32& color)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	T* newEntity = new T();

	Entity* entity = newEntity;
	entity->SetGeo(pGeo);
	pGeo->SetScale(scale);
	pGeo->SetPosition({ 0, 0, 0 });
	pGeo->SetRotation({ 0, 0, 0 });
	pGeo->SetDefaultColor(color);

	entity->Initialize(color);

	mpGameManager->mEntitiesToAdd.push_back(newEntity);

	return newEntity;
}