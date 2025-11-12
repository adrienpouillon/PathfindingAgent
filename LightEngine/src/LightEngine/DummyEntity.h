#pragma once
#include "Entity.h"

class DummyEntity : public Entity
{
public:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnCollision(Entity* other) override;
};

