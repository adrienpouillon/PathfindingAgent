#pragma once
#include "Agent.h"
class Assassin :public Agent
{
protected:
	Agent* mEntityTarget = nullptr;
	int mIndexPathEntityTarget;

	bool mIsTracking = false;
public:
	virtual void OnInitialize() { Agent::OnInitialize(); SetGhost(true); mEntityTarget = nullptr; mIndexPathEntityTarget = -1; }

	virtual void OnUpdate();

	virtual void OnPathFinish();

	virtual void OnCollision(Entity* collidedWith);

	void DeleteFollowPath();
	void ResetEntityTarget();
};

