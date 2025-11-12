#include "Entity.h"

#include "GameManager.h"
#include "Utils.h"
#include "Debug.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

void Entity::Initialize(const sf::Color& color)
{
	mDirection = sf::Vector2f(0.0f, 0.0f);

	mpShape->setOrigin(0.f, 0.f);
	mpShape->setFillColor(color);
	
	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Repulse(Entity* other) 
{
	if (mShapeTag == (int)ShapeType::Circle)
	{
		sf::CircleShape* current = dynamic_cast<sf::CircleShape*>(mpShape);
		sf::CircleShape* currentOther = dynamic_cast<sf::CircleShape*>(other->mpShape);

		sf::Vector2f distance = GetPosition() - other->GetPosition();

		float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);
		float length = std::sqrt(sqrLength);

		float radius1 = current->getRadius();
		float radius2 = currentOther->getRadius();

		float overlap = (length - (radius1 + radius2)) * 0.5f;

		sf::Vector2f normal = distance / length;

		sf::Vector2f translation = overlap * normal;

		sf::Vector2f position1 = GetPosition() - translation;
		sf::Vector2f position2 = other->GetPosition() + translation;

		SetPosition(position1.x, position1.y);
		other->SetPosition(position2.x, position2.y);
	}
}

bool Entity::IsColliding(Entity* other) const
{
	if (mShapeTag == (int)ShapeType::Circle)
	{
		sf::CircleShape* current = dynamic_cast<sf::CircleShape*>(mpShape);
		sf::CircleShape* currentOther = dynamic_cast<sf::CircleShape*>(other->mpShape);

		sf::Vector2f distance = GetPosition() - other->GetPosition();

		float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

		float radius1 = current->getRadius();
		float radius2 = currentOther->getRadius();

		float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

		return sqrLength < sqrRadius;
	}
}

bool Entity::IsInside(float x, float y) const
{
	if (mShapeTag == (int)ShapeType::Circle)
	{
		sf::CircleShape* current = dynamic_cast<sf::CircleShape*>(mpShape);

		sf::Vector2f position = GetPosition();

		float dx = x - position.x;
		float dy = y - position.y;

		float radius = current->getRadius();

		return (dx * dx + dy * dy) < (radius * radius);
	}
	else if (mShapeTag == (int)ShapeType::Rectangle)
	{
		sf::RectangleShape* current = dynamic_cast<sf::RectangleShape*>(mpShape);

		sf::Vector2f position = GetPosition();
		sf::Vector2f size = current->getSize();

		if (x >= position.x - size.x * 0.5f && x <= position.x + size.x * 0.5f)
		{
			if (y >= position.y - size.y * 0.5f && y <= position.y + size.y * 0.5f)
			{
				return true;
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

void Entity::SetPosition(float x, float y)
{
	sf::Vector2f size = GetSize();

	x -= size.x * 0.5f;
	y -= size.y * 0.5f;

	mpShape->setPosition(x, y);


	//#TODO Optimise
	if (mTarget.isSet) 
	{
		sf::Vector2f position = GetPosition();
		mTarget.distance = Utils::GetDistance(position.x, position.y, mTarget.position.x, mTarget.position.y);
		GoToDirection(mTarget.position.x, mTarget.position.y);
		mTarget.isSet = true;
	}
}

const sf::Vector2f& Entity::GetSize() const
{
	sf::Vector2f size = { 0, 0 };

	if (mShapeTag == (int)ShapeType::Circle)
	{
		sf::CircleShape* current = dynamic_cast<sf::CircleShape*>(mpShape);

		size.x = current->getRadius() * 2;
		size.y = size.x;
	}
	else if (mShapeTag == (int)ShapeType::Rectangle)
	{
		sf::RectangleShape* current = dynamic_cast<sf::RectangleShape*>(mpShape);

		size = current->getSize();
	}

	return size;
}

sf::Vector2f Entity::GetPosition() const
{
	sf::Vector2f size = GetSize();
	sf::Vector2f position = mpShape->getPosition();

	position.x += size.x * 0.5f;
	position.y += size.y * 0.5f;

	return position;
}

bool Entity::GoToDirection(int x, int y, float speed)
{
	sf::Vector2f position = GetPosition();
	sf::Vector2f direction = sf::Vector2f(x - position.x, y - position.y);
	
	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;

	SetDirection(direction.x, direction.y, speed);

	return true;
}

bool Entity::GoToPosition(int x, int y, float speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	sf::Vector2f position = GetPosition();

	mTarget.position = { x, y };
	mTarget.distance = Utils::GetDistance(position.x, position.y, x, y);
	mTarget.isSet = true;

	return true;
}

void Entity::SetDirection(float x, float y, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	mDirection = sf::Vector2f(x, y);
	mTarget.isSet = false;
}

void Entity::Update()
{
	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	sf::Vector2f translation = distance * mDirection;
	mpShape->move(translation);

	if (mTarget.isSet) 
	{
		float x1 = GetPosition().x;
		float y1 = GetPosition().y;

		float x2 = x1 + mDirection.x * mTarget.distance;
		float y2 = y1 + mDirection.y * mTarget.distance;

		Debug::DrawLine(x1, y1, x2, y2, sf::Color::Cyan);

		Debug::DrawCircle(mTarget.position.x, mTarget.position.y, 5.f, sf::Color::Magenta);

		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y);
			mDirection = sf::Vector2f(0.f, 0.f);
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