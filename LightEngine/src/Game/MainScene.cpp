#include "MainScene.h"
#include "Cursor.h"
#include <iostream>

void MainScene::Clear()
{
	delete pCursor;
	pCursor = nullptr;
}

void MainScene::OnInitialize()
{
	pCursor = new Cursor();
}

void MainScene::OnEvent(const sf::Event& event)
{
}

void MainScene::OnUpdate()
{
	pCursor->Update();
	pCursor->DisplayCoords();
}
