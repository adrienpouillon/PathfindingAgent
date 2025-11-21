#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

enum Tag
{

    //agent
    AGENT,
    CIVIL,
    GUARD,
    ASSASSIN
};

enum class ShapeType
{
    Circle,
    Rectangle,

    Count
};

namespace sf 
{
	class Shape;
    class Color;
}

class Scene;

class Entity
{
    struct Target 
    {
		sf::Vector2i position;
        float distance;
		bool isSet;
    };

protected:
    sf::Shape* mpShape;
    sf::Vector2f mDirection;
	Target mTarget;
    float mSpeed = 0.f;
    bool mToDestroy = false;
    int mTag = -1;
    int mShapeTag = -1;
	bool mRigidBody = false;

public:
	bool GoToDirection(int x, int y, float speed = -1.f);
    bool GoToPosition(int x, int y, float speed = -1.f);
    void SetPosition(float x, float y);
	void SetDirection(float x, float y, float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }
    void SetShapeTag(int shapeTag) { mShapeTag = shapeTag; }
	void SetRigidBody(bool isRigitBody) { mRigidBody = isRigitBody; }
	bool IsRigidBody() const { return mRigidBody; }
	Target GetTarget() const { return mTarget; }

    const sf::Vector2f& GetSize() const;

    template<typename TShape>
    TShape* CreateShape() { TShape* t = new TShape(); mpShape = t; return t; }
    sf::Vector2f GetPosition() const;
	sf::Shape* GetShape() { return mpShape; }

    bool IsShapeTag(int tag) const { return mShapeTag == tag; }
	bool IsTag(int tag) const { return mTag == tag; }
    bool IsType(int tag) const { return mTag >= tag; }
    bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y) const;

    void Destroy();
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    template<typename T>
    T* CreateCircleEntity(float radius, const sf::Color& color);

    template<typename T>
    T* CreateRectEntity(float width, float height, const sf::Color& color);

protected:
    Entity() = default;
    ~Entity() { delete mpShape; }

    virtual void OnUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void Update();
	void Initialize(const sf::Color& color);
	void Repulse(Entity* other);

    friend class GameManager;
    friend Scene;
};

#include "Entity.inl"