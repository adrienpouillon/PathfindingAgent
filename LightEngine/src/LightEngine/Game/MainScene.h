#pragma once
#include "../Scene.h"

class Cursor;

class MainScene : public Scene
{
	Cursor* pCursor = nullptr;
public:
	void Clear();

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};

