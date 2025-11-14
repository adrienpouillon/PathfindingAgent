#include "MainScene.h"
#include "Grid.h"
#include "Agent.h"
#include "Functions.h"
#include <iostream>


void MainScene::OnInitialize()
{
	SetGridDimensions(20, 10);

	mView.setSize(1920, 1080);

	mpGrid = new Grid();
}

void MainScene::OnEvent(const sf::Event& event)
{
	auto win = GameManager::Get()->GetWindow();
	sf::Vector2f worldMousePos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));

	InputManager(worldMousePos, event);
}

void MainScene::OnUpdate()
{
	ZoomManager();
	HandleGridSave();
	HandleGridResizing();

	GameManager::Get()->GetWindow()->setView(mView);

	mpGrid->Update();

	if (mSelectedEntity != nullptr)
	{
		sf::Vector2f pos = mSelectedEntity->GetPosition();
		Debug::DrawCircle(pos.x, pos.y, 5.f, sf::Color::Magenta);
	}
}

void MainScene::InputManager(sf::Vector2f worldMousePos, const sf::Event& e)
{
	static bool isPressed = false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == false)
	{
		isPressed = false;
	}
	
	Node<Cell>* nodeEnd = GetNode<Cell>(worldMousePos, GetGrid());

	if (isPressed == false)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				isPressed = true;
				//creer une entity

				Cell* nearest = GetNearestCell(worldMousePos, GetGrid()->GetAllCells());

				if (nearest->GetAgent() == false && nearest->GetObstacle() == false)
				{
					sf::Vector2f pos = nearest->getPosition();

					CreateAgent(pos, 100.f, 25.f, sf::Color::Cyan);
				}
			}

			return;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Entity* clicOnEntity = GetNearestEntity(worldMousePos);

			if (mSelectedEntity == nullptr)
			{
				if (clicOnEntity != nullptr)
				{
					//selectionner une entity
					mSelectedEntity = clicOnEntity;

					std::cout << "Selected !\n";
				}
			}
			else
			{
				if (clicOnEntity == nullptr)
				{
					//deplacer une entity
					if (Agent* a = dynamic_cast<Agent*>(mSelectedEntity))
					{
						a->GoToCell(worldMousePos, GetGrid());
						mSelectedEntity = nullptr;

						std::cout << "Moving !\n";
					}
				}
			}

			return;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		SetCellObstacle(worldMousePos, true);
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (Entity* e = GetNearestEntity(worldMousePos))
		{
			mSelectedEntity = nullptr;
			e->Destroy();
			return;
		}

		SetCellObstacle(worldMousePos, false);

		return;
	}
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
		if (mGridRows > 1)
		{
			mGridRows -= 1;
		}

		if (mGridCols > 1)
		{
			mGridCols -= 1;
		}

		CleanEntities();

		mpGrid->InitTab(mGridRows, mGridCols);
		mIsResizing = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
		if (mGridRows < 50)
		{
			mGridRows += 1;
		}

		if (mGridCols < 50)
		{
			mGridCols += 1;
		}

		CleanEntities();
		mpGrid->InitTab(mGridRows, mGridCols);
		mIsResizing = true;
	}
}

Agent* MainScene::CreateAgent(sf::Vector2f pos, float speed, int radius, sf::Color color)
{
	Agent* newEntity = CreateCircleEntity<Agent>(radius, color);
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

void MainScene::Clear()
{
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

void MainScene::ZoomManager()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		mView.zoom(1.f - GetDeltaTime());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		mView.zoom(1.f + GetDeltaTime());
	}
}

void MainScene::SetCellObstacle(sf::Vector2f pos, bool state)
{
	sf::Vector2f fixedPos = { 0, 0 };

	Cell* nearest = GetNearestCell(pos, GetGrid()->GetAllCells());

	if (nearest != nullptr)
	{
		if (nearest->GetAgent() == false)
		{
			nearest->SetObstacle(state);
		}
	}
}


