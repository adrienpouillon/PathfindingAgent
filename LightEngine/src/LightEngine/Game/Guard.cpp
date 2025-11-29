#include "Guard.h"
#include "MainScene.h"

void Guard::OnUpdate()
{
	int sizePath = mPath.GetPath()->size();
	Agent::OnUpdate();

	int pathSize = mPath.GetPath()->size();
	if (sizePath != pathSize)
	{
		IncreaseCoin(1);
	}

	if (mCurrentScene->GetUseCoin())
	{
		if (pathSize > 0)
		{
			int cost = 5;
			if (mCoin >= cost)
			{
				mCurrentScene->CreateCivil(GetPosition(), 75.f, RADIUS, sf::Color::Yellow, sf::Color(64, 128, 128));
				IncreaseCoin(-cost);
			}
		}
	}
}

void Guard::OnPathFinish()
{
	Agent::OnPathFinish();
}
