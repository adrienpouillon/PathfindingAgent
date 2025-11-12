#include "MainScene.h"
#include "Cursor.h"
#include "Grid.h"
#include <iostream>

void MainScene::Clear()
{
	delete mpCursor;
	mpCursor = nullptr;

	delete mpGrid;
	mpGrid = nullptr;
}

void MainScene::OnInitialize()
{
	SetGridSize(20, 20);

	mView.setSize(1920, 1080);
	mpGrid = new Grid<Cell>(50);

	mpCursor = new Cursor();
}

void MainScene::OnEvent(const sf::Event& event)
{
}

void MainScene::OnUpdate()
{
	GameManager::Get()->GetWindow()->setView(mView);

	mpCursor->Update();
	mpGrid->Update();
}
