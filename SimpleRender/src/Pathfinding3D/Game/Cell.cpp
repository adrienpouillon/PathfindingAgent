#include "pch.h"
#include "Cell.h"
#include "../LightEngine/GameManager.h"
#include "../LightEngine/Entity.h"
#include "../LightEngine/Debug.h"
#include "Agent.h"
#include <iostream>

void Cell::OnUpdate()
{
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

	mpGeo->SetScale({ scale.x, CellHeight * mConfigHeight, scale.z });
	mpGeo->SetPosition({ pos.x, CellHeight * (mConfigHeight - 1), pos.z });
}
