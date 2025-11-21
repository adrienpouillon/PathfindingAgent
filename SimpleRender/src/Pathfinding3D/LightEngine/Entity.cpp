#include "pch.h"

#include "Entity.h"

#include "GameManager.h"
#include "Utils.h"
#include "Debug.h"

void Entity::Initialize(const gce::Vector3f32& color)
{
	mDirection = gce::Vector3f32(0.0f, 0.0f, 0.0f);
	mTarget.isSet = false;

	OnInitialize();
}

bool Entity::IsColliding(Entity* other) const
{
	if (Sphere* s1 = dynamic_cast<Sphere*>(mpGeo))
	{
		if (Sphere* s2 = dynamic_cast<Sphere*>(other->GetGeo()))
		{
			gce::Vector3f32 distance = s1->GetPosition() - s2->GetPosition();

			float sqrLength = (distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z);

			float radius1 = s1->GetScale().x * 0.5f;
			float radius2 = s2->GetScale().x * 0.5f;

			float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

			return sqrLength < sqrRadius;
		}
	}

	return false;
}

bool Entity::IsInside(float x, float y, float z) const
{
	if (Sphere* s1 = dynamic_cast<Sphere*>(mpGeo))
	{
		gce::Vector3f32 position = GetPosition();

		float dx = x - position.x;
		float dy = y - position.y;
		float dz = z - position.z;

		float radius = s1->GetScale().x;

		return (dx * dx + dy * dy + dz * dz) < (radius * radius);
	}
	else if (Cube* s1 = dynamic_cast<Cube*>(mpGeo))
	{
		gce::Vector3f32 position = GetPosition();
		gce::Vector3f32 size = s1->GetScale();

		if (x >= position.x - size.x * 0.5f && x <= position.x + size.x * 0.5f)
		{
			if (y >= position.y - size.y * 0.5f && y <= position.y + size.y * 0.5f)
			{
				if (z >= position.z - size.z * 0.5f && z <= position.z + size.z * 0.5f)
				{
					return true;
				}
			}
		}

		return false;
	}

	return false;
}

void Entity::Destroy()
{
	mToDestroy = true;

	OnDestroy();
}

void Entity::SetPosition(float x, float y, float z)
{
	gce::Vector3f32 size = GetSize();

	mpGeo->SetPosition(gce::Vector3f32(x, y, z ));

	//#TODO Optimise
	if (mTarget.isSet) 
	{
		gce::Vector3f32 position = GetPosition();
		mTarget.distance = Utils::GetDistance(position.x, position.y, position.z, mTarget.position.x, mTarget.position.y, mTarget.position.z);
		GoToDirection(mTarget.position.x, mTarget.position.y, mTarget.position.z);
		mTarget.isSet = true;
	}
}

const gce::Vector3f32& Entity::GetSize() const
{
	return mpGeo->GetScale();
}

gce::Vector3f32 Entity::GetPosition() const
{
	return mpGeo->GetPosition();
}

bool Entity::GoToDirection(float x, float y, float z, float speed)
{
	gce::Vector3f32 position = GetPosition();
	gce::Vector3f32 direction = gce::Vector3f32(x - position.x, y - position.y, z - position.z);
	
	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;

	SetDirection(direction.x, direction.y, direction.z, speed);

	return true;
}

bool Entity::GoToPosition(float x, float y, float z, float speed)
{
	if (GoToDirection(x, y, z, speed) == false)
		return false;

	gce::Vector3f32 position = GetPosition();

	mTarget.position = { x, y, z };
	mTarget.distance = Utils::GetDistance(position.x, position.y, position.z, x, y, z);
	mTarget.isSet = true;

	return true;
}

void Entity::SetDirection(float x, float y, float z, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	mDirection = gce::Vector3f32(x, y, z);
	mTarget.isSet = false;
}

void Entity::Update()
{
	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	gce::Vector3f32 translation = mDirection * distance;
	mpGeo->Translate(translation);

	if (mTarget.isSet) 
	{
		float x1 = GetPosition().x;
		float y1 = GetPosition().y;
		float z1 = GetPosition().z;

		float x2 = x1 + mDirection.x * mTarget.distance;
		float y2 = y1 + mDirection.y * mTarget.distance;
		float z2 = z1 + mDirection.z * mTarget.distance;

		//Debug::DrawLine(x1, y1, z1, x2, y2, z2, gce::Vector3f32(0.f, 1.f, 1.f));

		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y, mTarget.position.z);
			mDirection = gce::Vector3f32(0.f, 0.f, 0.f);
			mTarget.isSet = false;
		}
	}

	OnUpdate();
}

Scene* Entity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}

void Entity::ResetGeometryColor()
{
	mpGeo->SetColor(mpGeo->GetDefaultColor());
}
