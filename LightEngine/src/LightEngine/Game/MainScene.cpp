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

void MainScene::CleanEntities()
{
	for (Entity* e : GameManager::Get()->GetEntities())
	{
		e->Destroy();
	}
}

void MainScene::OnInitialize()
{
	SetGridSize(20, 20);

	mView.setSize(1920, 1080);
	mpGrid = new Grid<Cell>(50);
	mpGrid->InitGridFromTxt("grid.txt");

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

	HandleGridSave();
	HandleGridResizing();

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
	pathFinding.SetCursor(mpCursor);
	pathFinding.SetPathFinish(false);
	pathFinding.SetStartNode(nullptr);
	pathFinding.SetEndNode(nullptr);
	return pathFinding;
}

void MainScene::HandleGridSave()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) == false)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) == false)
		{
			mIsSaving = false;
		}
	}

	if (mIsSaving == true)
	{
		sf::Vector2f center = mView.getCenter();
		Debug::DrawText(center.x, center.y, "Grid Saved !", 0.5f, 0.5f, 40, sf::Color::Green);
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{

			std::cout << "SAVED !\n";

			mpGrid->SaveGrid("grid.txt");

			mIsSaving = true;
		}
	}
}

void MainScene::HandleGridResizing()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) == false && sf::Keyboard::isKeyPressed(sf::Keyboard::F2) == false)
	{
		mIsResizing = false;
	}

	if (mIsResizing == true)
	{
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		CleanEntities();
		mpGrid->CreateEmptyGrid(mGridRows + 1, mGridCols + 1);
		mIsResizing = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
		CleanEntities();
		mpGrid->CreateEmptyGrid(mGridRows - 1, mGridCols - 1);
		mIsResizing = true;
	}
}
