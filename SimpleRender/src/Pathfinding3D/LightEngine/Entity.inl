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
inline T* Entity::CreateEntity(Geometry* pGeo, gce::Vector3f32 scale, const gce::Vector3f32& color)
{
	return GetScene()->CreateEntity<T>(pGeo, scale, color);
}