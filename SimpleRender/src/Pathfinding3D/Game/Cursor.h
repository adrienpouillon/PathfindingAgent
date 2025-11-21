#pragma once
#include "../LightEngine/Entity.h"

class MainScene;

class Cursor : public Entity
{
	int mFollowRows = 0, mFollowCols = 0;

	MainScene* mpCurrentScene = nullptr;
public:
	void OnInitialize() override;
	void OnUpdate() override;
	void OnCollision(Entity* other) override;

	void ResetFollowGridCoords() { mFollowRows = 0; mFollowCols = 0; SetPosition(0, 0, 0); }

	void HandleInputs();
};

