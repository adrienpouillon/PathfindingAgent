#include "pch.h"

#include "GameManager.h"

#include "Entity.h"
#include "Debug.h"

#include <iostream>
#include <chrono>

GameManager::GameManager()
{
	mpWindow = nullptr;
	mDeltaTime = 0.0f;
	mpScene = nullptr;
	mpCam = nullptr;
	mWindowWidth = -1;
	mWindowHeight = -1;
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	delete mpCam;
	delete mpScene;

	for (Entity* entity : mEntities)
	{
		delete entity;
	}
}

void GameManager::InstanceWindow(unsigned int width, unsigned int height, std::wstring title)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new Window(title, width, height);

	mpCam = new Camera(CameraType::PERSPECTIVE);
	mpCam->SetFOV(gce::PI / 2);
	mpCam->SetNearPlane(0.1f);
	mpCam->SetFarPlane(2000.f);
	mpCam->SetPosition({ 0, 0, -10.f });
	mpCam->SetRotation({ 0.f, 0.0f, 0.0f });

	mWindowWidth = width;
	mWindowHeight = height;
}

void GameManager::Run()
{
	if (mpWindow == nullptr) 
	{
		std::cout << "Window not created, creating default window" << std::endl;
		InstanceWindow(1280, 720, L"Default window");
	}

	_ASSERT(mpScene != nullptr);

	while (mpWindow->IsOpen())
	{
		SetDeltaTime();
		Draw();
		Update();
	}
}

void GameManager::SetDeltaTime()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> delta = currentTime - lastTime;
	lastTime = currentTime;

	mDeltaTime = delta.count();
}

void GameManager::Update()
{
	mpScene->OnUpdate();

    //Update
    for (auto it = mEntities.begin(); it != mEntities.end(); )
    {
		Entity* entity = *it;

        entity->Update();

        if (entity->ToDestroy() == false)
        {
            ++it;
            continue;
        }

        mEntitiesToDestroy.push_back(entity);
        it = mEntities.erase(it);
    }

    //Collision
    for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
    {
        auto it2 = it1;
        ++it2;
        for (; it2 != mEntities.end(); ++it2)
        {
            Entity* entity = *it1;
            Entity* otherEntity = *it2;

            if (entity->IsColliding(otherEntity))
            {
                entity->OnCollision(otherEntity);
                otherEntity->OnCollision(entity);
            }
        }
    }

	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it) 
	{
		delete *it;
	}

    mEntitiesToDestroy.clear();

	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();
}

void GameManager::Draw()
{
	mpWindow->Begin(*mpCam);

	for (Entity* entity : mEntities)
	{
		if (entity->ToDestroy())
			continue;

		mpWindow->Draw(*entity->GetGeo());
	}

	Debug::Get()->Draw(mpWindow);

	mpWindow->End();
	mpWindow->Display();

	std::vector<Text*>& mpTexts = Debug::Get()->mpTexts;

	for (auto it = mpTexts.begin(); it != mpTexts.end();)
	{
		delete* it;

		it = mpTexts.erase(it);
	}

	mpTexts.clear();
}
