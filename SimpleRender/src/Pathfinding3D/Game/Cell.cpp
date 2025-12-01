#include "pch.h"
#include "Cell.h"

#include "../LightEngine/GameManager.h"
#include "../LightEngine/Entity.h"
#include "../LightEngine/Debug.h"

#include "MainScene.h"
#include "Agent.h"
#include <iostream>

void Cell::Start()
{ 
	SetHasAgent(false);
	SetHasCoin(false);
	SetConfigHeight(1);
	SetRowCol(-1, -1);
	mpAgent = nullptr;
	mEntityCoin = CreateCoin();
}

void Cell::OnUpdate()
{
	PrintCoin();
}

void Cell::CheckStatus(int cellSize)
{
	for (Entity* e : GameManager::Get()->GetEntities())
	{
		if (e->IsType(Tag::AGENT))
		{
			gce::Vector3f32 pos = GetPosition();
			gce::Vector3f32  ePos = e->GetPosition();

			gce::Vector3f32 entitySize = e->GetSize();

			if (ePos.x + entitySize.x * 0.5f > pos.x - cellSize * 0.5f && ePos.x - entitySize.x * 0.5f < pos.x + cellSize * 0.5f)
			{
				if (ePos.z + entitySize.z * 0.5f > pos.z - cellSize * 0.5f && ePos.z - entitySize.z * 0.5f < pos.z + cellSize * 0.5f)
				{
					if (Agent* a = dynamic_cast<Agent*>(e))
					{
						SetAgent(a);
					}
					else
					{
						SetAgent(nullptr);
					}

					SetHasAgent(true);
					return;
				}
			}
		}
	}

	SetAgent(nullptr);
	SetHasAgent(false);
}

void Cell::ChangeHeight(int newHeight)
{
	if (newHeight < 0)
		newHeight = 3;
	else if (newHeight > 3)
		newHeight = 0;

	mConfigHeight = newHeight;

	gce::Vector3f32 pos = mpGeo->GetPosition();
	gce::Vector3f32 scale = mpGeo->GetScale();

	mpGeo->SetScale({ scale.x, CELL_HEIGHT_UP * mConfigHeight, scale.z });
	mpGeo->SetPosition({ pos.x, CELL_HEIGHT_UP * (mConfigHeight - 1), pos.z });
}

void Cell::PrintCoin()
{
	if (mEntityCoin == nullptr)
	{
		mEntityCoin = CreateCoin();
	}

	if (HasCoin())
	{
		gce::Vector3f32 pos = mpGeo->GetPosition();
		mEntityCoin->SetPosition(pos.x, pos.y + CELL_HEIGHT_COIN, pos.z);
	}
	else
	{
		gce::Vector3f32 pos = mpGeo->GetPosition();
		if (pos.x != -5000.f || pos.y != -5000.f || pos.z != -5000.f)
		{
			mEntityCoin->SetPosition(-5000.f, -5000.f, -5000.f);
		}
	}
}

Entity* Cell::CreateCoin()
{
	MainScene* mainScene = GameManager::Get()->GetScene<MainScene>();
	int size = mainScene->GetGrid()->GetCellSize() * 0.30f;
	return mainScene->CreateEntity<Entity>(new Sphere(), gce::Vector3f32(size, size, size), gce::Vector3f32(1.f, 0.5f, 0.f));
}
