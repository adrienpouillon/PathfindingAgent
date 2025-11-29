#pragma once
#include "Agent.h"
class Guard :public Agent
{
protected:

public:
	virtual void OnInitialize() { Agent::OnInitialize(); SetCoin(0); }

	virtual void OnUpdate();

	virtual void OnPathFinish();

};

