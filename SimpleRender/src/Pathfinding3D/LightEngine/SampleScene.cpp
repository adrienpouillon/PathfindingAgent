#include "pch.h"

#include "SampleScene.h"
#include "Entity.h"

#include "DummyEntity.h"

#include "Debug.h"

void SampleScene::OnInitialize()
{
	pEntity1 = CreateEntity<DummyEntity>(new Cube(), gce::Vector3f32(10, 10, 10), gce::Vector3f32(1, 0, 0));
	pEntity1->SetPosition(0, 0, 0);

	pEntitySelected = nullptr;
}

void SampleScene::TrySetSelectedEntity(Entity* pEntity, int x, int y, int z)
{
	if (pEntity->IsInside(x, y, z) == false)
		return;

	pEntitySelected = pEntity;
}

void SampleScene::OnUpdate()
{
}