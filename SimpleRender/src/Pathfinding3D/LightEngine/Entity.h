#pragma once
#include "pch.h"

enum Tag
{
    CURSOR,

    AGENT,
    CIVIL,
    GUARD,
    ASSASSIN,

    Count
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
		gce::Vector3f32 position;
        float distance;
		bool isSet;
    };

protected:
    Geometry* mpGeo = nullptr;
    gce::Vector3f32 mDirection;
	Target mTarget;
    float mSpeed = 0.f;
    bool mToDestroy = false;
    int mTag = -1;
    int mShapeTag = -1;
	bool mRigidBody = false;

public:
	bool GoToDirection(float x, float y, float z, float speed = -1.f);
    bool GoToPosition(float x, float y, float z, float speed = -1.f);
    void SetPosition(float x, float y, float z);
	void SetDirection(float x, float y, float z, float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }
    void SetShapeTag(int shapeTag) { mShapeTag = shapeTag; }
	void SetRigidBody(bool isRigitBody) { mRigidBody = isRigitBody; }
	bool IsRigidBody() const { return mRigidBody; }
	Target GetTarget() const { return mTarget; }

    Geometry* GetGeo() { return mpGeo; }
    void SetGeo(Geometry* geo) { mpGeo = geo; }

    const gce::Vector3f32& GetSize() const;

    template<typename TShape>
    TShape* CreateShape() { TShape* t = new TShape(); mpGeo = t; return t; }
    gce::Vector3f32 GetPosition() const;

    bool IsShapeTag(int tag) const { return mShapeTag == tag; }
	bool IsTag(int tag) const { return mTag == tag; }
    bool IsType(int type) const { return mTag >= type; }
    bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y, float z) const;

    void Destroy();
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    void ResetGeometryColor();

    template<typename T>
    T* CreateEntity(Geometry* pGeo, gce::Vector3f32 scale, const gce::Vector3f32& color);

protected:
    Entity() = default;
    ~Entity() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void Update();
	void Initialize(const gce::Vector3f32& color);

    friend class GameManager;
    friend Scene;
};

#include "Entity.inl"