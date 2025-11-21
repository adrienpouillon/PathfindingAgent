#include "pch.h"
#include "Assassin.h"
#include "MainScene.h"
#include "Civil.h"
#include "Guard.h"
#include "Functions.h"

void Assassin::OnUpdate()
{
	Agent::OnUpdate();

	if (mpCurrentScene->GetSelectedEntity() == this)
	{
		ResetEntityTarget();
		return;
	}

	Grid* pGrid = mpCurrentScene->GetGrid();
	float distanceVisibility = pGrid->GetCellSize() * 50;
	gce::Vector3f32 posAssasin = GetPosition();

	mEntityTarget = GetEntityNearest<Civil>(posAssasin, distanceVisibility);

	if (mEntityTarget != nullptr && mEntityTarget->ToDestroy() == false)
	{
		gce::Vector3f32 targetPos = mEntityTarget->GetPosition();

		float distance = Utils::GetDistance(targetPos.x, targetPos.y, targetPos.z, posAssasin.x, posAssasin.y, posAssasin.z);

		if (distance > distanceVisibility)
			DeleteFollowPath();

	}
	else
		DeleteFollowPath();
}

void Assassin::OnPathFinish()
{
	Agent::OnPathFinish();

	Grid* pGrid = mpCurrentScene->GetGrid();

	if (mEntityTarget != nullptr)
	{
		if (GetTarget().isSet)
			return;

		std::vector<Node<Cell>*>* allPathNearest = mEntityTarget->GetPathfinding()->GetPath();

		int length = allPathNearest->size();
		int max = length - 1;

		if (length > 0)
		{
			if (GetTarget().isSet)
				return;

			std::vector<Node<Cell>*>* allPathNearest = mEntityTarget->GetPathfinding()->GetPath();

			int length = allPathNearest->size();
			int max = length - 1;

			if (length > 0)
			{
				if (mIsTracking == false)
				{
					mIndexPathEntityTarget = max;
					mIsTracking = true;
				}

				if (mIndexPathEntityTarget > max) mIndexPathEntityTarget = max;
				if (mIndexPathEntityTarget < 0)
				{
					mEntityTarget = nullptr;
					mIsTracking = false;
					return;
				}

				GoToNode((*allPathNearest)[mIndexPathEntityTarget], pGrid);
				mIndexPathEntityTarget--;

				return;
			}
		}
	}

	mIsTracking = false;
}

void Assassin::OnCollision(Entity* collidedWith)
{
	if (Civil* c = dynamic_cast<Civil*>(collidedWith))
	{
		mEntityTarget = nullptr;
		collidedWith->Destroy();
	}
	else if (Guard* c = dynamic_cast<Guard*>(collidedWith))
	{
		Destroy();
	}
}

void Assassin::DeleteFollowPath()
{
	ResetEntityTarget();
	mPath.GetPath()->clear();
	mIndexPath = 0;
}

void Assassin::ResetEntityTarget()
{
	mEntityTarget = nullptr;
	mIndexPathEntityTarget = -1;
}


