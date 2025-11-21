#pragma once
#include "Agent.h"
class Guard :public Agent
{
protected:

public:
	virtual void OnInitialize() { Agent::OnInitialize(); }

	virtual void OnUpdate();

	virtual void OnPathFinish();

};

