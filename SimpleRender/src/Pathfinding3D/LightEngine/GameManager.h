#pragma once
#include "pch.h"

#include <list>

constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;

class Entity;
class Scene;
class Debug;

namespace sf 
{
	class RenderWindow;
	class Event;
}

class GameManager
{
	std::list<Entity*> mEntities;
	std::list<Entity*> mEntitiesToDestroy;
	std::list<Entity*> mEntitiesToAdd;

	Window* mpWindow = nullptr;
	Camera* mpCam = nullptr;

	Scene* mpScene = nullptr;

	float mDeltaTime;

	int mWindowWidth = 0;
	int mWindowHeight = 0;

private:
	GameManager();

	void Run();

	void Update();
	void Draw();

	void SetDeltaTime();

public:
	// LightEngine
	~GameManager();
	static GameManager* Get();

	void InstanceWindow(unsigned int width, unsigned int height, std::wstring title);
	Window* GetWindow() const { return mpWindow; }
	Camera* GetCamera() { return mpCam; }

	std::list<Entity*>& GetEntities() { return mEntities; }

	template<typename T>
	void LaunchScene();

	float GetDeltaTime() const { return mDeltaTime; }
	Scene* GetScene() const { return mpScene; }

	template<typename T>
	T* GetScene() { if (T* t = dynamic_cast<T*>(mpScene)) return t; return nullptr; }

	friend Debug;
	friend Scene;
	/////////////
};

#include "GameManager.inl"