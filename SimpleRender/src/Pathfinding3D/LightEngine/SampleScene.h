#pragma once

#include "Scene.h"

class Entity;

class SampleScene : public Scene
{
	Entity* pEntity1;
	Entity* pEntity2;

	Entity* pEntitySelected;

private:
	void TrySetSelectedEntity(Entity* pEntity, int x, int y, int z);

public:
	void OnInitialize() override;
	void OnUpdate() override;
};


