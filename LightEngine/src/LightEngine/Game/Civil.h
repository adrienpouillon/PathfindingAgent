#pragma once
#include "Agent.h"

class Civil : public Agent
{
protected:

public:
	virtual void OnInitialize() { Agent::OnInitialize(); }

	virtual void OnUpdate();

	virtual void OnPathFinish();

	int GetRandomNumber(int min, int max);
};

