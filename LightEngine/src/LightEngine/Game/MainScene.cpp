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
	mpGrid = new Grid<Cell>();
	mpGrid->SetCellSize(100);

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		mView.zoom(1.f - GetDeltaTime());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		mView.zoom(1.f + GetDeltaTime());
	}
}

Agent<Cell>* MainScene::CreateAgent(sf::Vector2f pos, float speed, int radius, sf::Color color)
{
	Agent<Cell>* newEntity = CreateCircleEntity<Agent<Cell>>(radius, color);
	newEntity->SetRoam(false);
	newEntity->SetTag(AGENT);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());

	newEntity->SetPosition(pos.x, pos.y);

	return newEntity;
}

PathFinding<Cell> MainScene::CreatePathFinding()
{
	PathFinding<Cell> pathFinding = PathFinding<Cell>();
	pathFinding.SetReadFinish(true);
	return pathFinding;
}
