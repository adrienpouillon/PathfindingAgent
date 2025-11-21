#include "MainScene.h"
#include "Grid.h"
#include "Agent.h"
#include "Civil.h"
#include "Guard.h"
#include "Assassin.h"
#include "Functions.h"
#include <iostream>


void MainScene::OnInitialize()
{
	mGridRows = 9;
	mGridCols = 9;
	mSelectedEntity = nullptr;
	mIsSaving = false;
	mIsResizing = false;
	mRestartNode = false;
	mIsChangingGridConfig = false;

	SetDrawDebug(DRAW_DEBUG_AGENT_NODE);

	mView.setSize(1920, 1080);

	mpGrid = new Grid();
	mpGrid->SetParameters(50, true);

	srand(time(nullptr));
}

void MainScene::OnEvent(const sf::Event& event)
{
	ResetNode();

	auto win = GameManager::Get()->GetWindow();
	sf::Vector2f worldMousePos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));

	float cellSize = mpGrid->GetCellSize();
	float offsetCell = -cellSize * 0.5f;

	int actionExecute = 0;

	if (event.type == sf::Event::KeyPressed)
	{
		KeyPressedInputs(event, worldMousePos, actionExecute);
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		KeyReleasedInputs(event, worldMousePos, actionExecute);
	}
	KeyInputs(event, worldMousePos, actionExecute);

	// Don't interact if you're not on the grid
	if (worldMousePos.x <= offsetCell || worldMousePos.x >= offsetCell + mGridCols * cellSize
		|| worldMousePos.y <= offsetCell || worldMousePos.y >= offsetCell + mGridRows * cellSize)
	{
		return;
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		MousePressedInputs(event, worldMousePos, actionExecute);
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		MouseReleasedInputs(event, worldMousePos, actionExecute);
	}
	MouseInputs(event, worldMousePos, actionExecute);

}

void MainScene::OnUpdate()
{
	DisplayUI();
	ZoomManager();

	GameManager::Get()->GetWindow()->setView(mView);
	mpGrid->Update();

	DrawSelectedEntity();

	if (mDrawDebug == DRAW_DEBUG_AGENT_NODE)
	{
		DebugDrawPath();
	}
}

void MainScene::MousePressedInputs(const sf::Event& event, sf::Vector2f worldMousePos, int& actionExecute)
{
	float radius = 25.f;

	switch (event.mouseButton.button)
	{
	case sf::Mouse::Button::Left:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && actionExecute < 1)
		{
			mRestartNode = true;
			//ajouter un agent
			Cell* nearest = GetNearestCell(worldMousePos, GetGrid()->GetAllCells());
			if (nearest->HasAgent() == false && nearest->IsObstacle() == false)
			{
				sf::Vector2f pos = nearest->getPosition();
				CreateGuard(pos, 100.f, radius, sf::Color::Cyan, sf::Color::Blue);
				actionExecute++;
			}
		}

		if (actionExecute < 1)
		{
			Entity* clicOnEntity = GetNearestEntity(worldMousePos);
			if (clicOnEntity == mSelectedEntity)
			{
				mSelectedEntity = nullptr;
				actionExecute++;
			}
			else if (clicOnEntity == nullptr)
			{
				//deplacer une entity
				if (Agent* a = dynamic_cast<Agent*>(mSelectedEntity))
				{
					a->GoToCell(worldMousePos, GetGrid());
					actionExecute++;
					std::cout << "Moving !\n";
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) == false && actionExecute < 1)
				{
					mSelectedEntity = nullptr;
					actionExecute++;
				}
			}
			else
			{
				mSelectedEntity = clicOnEntity;
				actionExecute++;
				//std::cout << "Selected !\n";
			}
		}
		break;
	case sf::Mouse::Button::Middle:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && actionExecute < 1)
		{
			mRestartNode = true;
			//ajouter un agent
			Cell* nearest = GetNearestCell(worldMousePos, GetGrid()->GetAllCells());
			if (nearest->HasAgent() == false && nearest->IsObstacle() == false)
			{
				sf::Vector2f pos = nearest->getPosition();
				CreateCivil(pos, 75.f, radius, sf::Color::Yellow, sf::Color(64, 128, 128));
				actionExecute++;
			}
		}
		break;
	case sf::Mouse::Button::Right:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && actionExecute < 1)
		{
			mRestartNode = true;
			//ajouter un agent
			Cell* nearest = GetNearestCell(worldMousePos, GetGrid()->GetAllCells());
			if (nearest->HasAgent() == false && nearest->IsObstacle() == false)
			{
				sf::Vector2f pos = nearest->getPosition();
				CreateAssassin(pos, 100.f, radius, sf::Color::Red, sf::Color::Magenta);
				actionExecute++;
			}
		}
		break;
	}
}

void MainScene::MouseReleasedInputs(const sf::Event & event, sf::Vector2f worldMousePos, int& actionExecute)
{

}

void MainScene::KeyPressedInputs(const sf::Event & event, sf::Vector2f worldMousePos, int& actionExecute)
{
	if (mSelectedEntity != nullptr)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && actionExecute < 1)
		{
			if (Agent* a = dynamic_cast<Agent*>(mSelectedEntity))
			{
				a->SetRoam(!a->GetRoam());
				actionExecute++;
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H) && actionExecute < 1)
	{
		CleanEntities();
		actionExecute++;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && actionExecute < 1)
	{
		mRestartNode = true;
		CleanWalls();
		actionExecute++;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && actionExecute < 1)
	{
		if (mDrawDebug > DRAW_DEBUG_NO - 1)
		{
			mDrawDebug = 0;
		}
		else
		{
			mDrawDebug++;
		}
		actionExecute++;
	}
}

void MainScene::KeyReleasedInputs(const sf::Event & event, sf::Vector2f worldMousePos, int& actionExecute)
{

}

void MainScene::MouseInputs(const sf::Event & event, sf::Vector2f worldMousePos, int& actionExecute)
{
	switch (event.mouseButton.button)
	{
	case sf::Mouse::Button::Left:

		break;
	case sf::Mouse::Button::Middle:
		if (actionExecute < 1 && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			mRestartNode = true;
			SetCellObstacle(worldMousePos, true);
			actionExecute++;
		}
		break;
	case sf::Mouse::Button::Right:
		if (actionExecute < 1 && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			mRestartNode = true;

			if (Entity* e = GetNearestEntity(worldMousePos))
			{
				mSelectedEntity = nullptr;
				e->Destroy();
				actionExecute++;
			}
			SetCellObstacle(worldMousePos, false);
			actionExecute++;
		}
		break;
	}
	
}

void MainScene::KeyInputs(const sf::Event & event, sf::Vector2f worldMousePos, int& actionExecute)
{
	HandleGridSave();
	HandleChangingGridConfig();
	HandleCameraTransformations();
}

void MainScene::HandleCameraTransformations()
{
	HandleCamMove();
	HandleCamZoom();
}

void MainScene::HandleCamZoom()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) == false && sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) == false)
	{
		mIsResizing = false;
	}

	if (mIsResizing == true)
		return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
	{
		if (mGridRows > 1)
			mGridRows -= 1;


		if (mGridCols > 1)
			mGridCols -= 1;

		CleanEntities();

		mpGrid->InitTab(mGridRows, mGridCols);
		mIsResizing = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
	{
		if (mGridRows < 50)
			mGridRows += 1;

		if (mGridCols < 50)
			mGridCols += 1;

		CleanEntities();
		mpGrid->InitTab(mGridRows, mGridCols);
		mIsResizing = true;
	}
}

void MainScene::HandleCamMove()
{
	// The speed will be reduced when you zoom in
	float ratio = mView.getSize().x / (float)WINDOW_WIDTH; // Ratio

	float dt = GetDeltaTime();

	float finalSpeedDt = CamSpeed * ratio * dt;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		mView.move(sf::Vector2f(-finalSpeedDt, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		mView.move(sf::Vector2f(finalSpeedDt, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		mView.move(sf::Vector2f(0, -finalSpeedDt));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		mView.move(sf::Vector2f(0, finalSpeedDt));
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

			mpGrid->SaveGrid(GridConfigs::GetConfigString());

			mIsSaving = true;
		}
	}
}

void MainScene::HandleChangingGridConfig()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) == false && sf::Keyboard::isKeyPressed(sf::Keyboard::F2) == false)
	{
		mIsChangingGridConfig = false;
	}

	if (mIsChangingGridConfig)
		return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		mIsChangingGridConfig = true;
		SwitchGridFile(-1);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
		mIsChangingGridConfig = true;
		SwitchGridFile(+1);
	}
}

Agent* MainScene::CreateAgent(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor)
{
	Agent* newEntity = CreateCircleEntity<Agent>(radius, classicColor);
	newEntity->SetRoam(false);
	newEntity->SetTag(AGENT);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y);

	return newEntity;
}

Civil* MainScene::CreateCivil(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor)
{
	Civil* newEntity = CreateCircleEntity<Civil>(radius, classicColor);

	//agent
	newEntity->SetRoam(false);
	newEntity->SetTag(CIVIL);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y);

	return newEntity;
}

Guard* MainScene::CreateGuard(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor)
{
	Guard* newEntity = CreateCircleEntity<Guard>(radius, classicColor);
	newEntity->SetRoam(false);
	newEntity->SetTag(GUARD);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y);

	return newEntity;
}

Assassin* MainScene::CreateAssassin(sf::Vector2f pos, float speed, int radius, sf::Color classicColor, sf::Color roamColor)
{
	Assassin* newEntity = CreateCircleEntity<Assassin>(radius, classicColor);
	newEntity->SetRoam(false);
	newEntity->SetTag(ASSASSIN);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y);

	return newEntity;
}

PathFinding<Cell> MainScene::CreatePathFinding()
{
	PathFinding<Cell> pathFinding;
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

	mSelectedEntity = nullptr;
}

void MainScene::CleanWalls()
{
	for (std::vector<Cell*>& allC : mpGrid->GetAllCells())
	{
		for (Cell* c : allC)
		{
			c->SetIsObstacle(false);
		}
	}
}

void MainScene::ZoomManager()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		mView.zoom(1.f - GetDeltaTime());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		mView.zoom(1.f + GetDeltaTime());
	}
}

void MainScene::DrawSelectedEntity()
{
	if (mSelectedEntity != nullptr && mSelectedEntity->ToDestroy() == false)
	{
		sf::Vector2f pos = mSelectedEntity->GetPosition();
		Debug::DrawCircle(pos.x, pos.y, 5.f, sf::Color::Magenta);
	}
}

void MainScene::DebugDrawPath()
{
	std::vector<Node<Cell>*> tabNode = GetGrid()->GetAllNodes();
	int lenghtI = tabNode.size();
	for (int i = 0; i < lenghtI; i++)
	{
		std::vector<NeighborsCost<Cell>> tabNeighbor = tabNode[i]->GetNeighborsCost();
		sf::Vector2f pos = tabNode[i]->GetData()->getPosition();
		int lenghtJ = tabNeighbor.size();
		for (int j = 0; j < lenghtJ; j++)
		{

			if (tabNeighbor[j].GetCost() > 2)
			{
				sf::Vector2f posN = tabNeighbor[j].GetNeighbor()->GetData()->getPosition();
				Debug::DrawLine(pos.x, pos.y, posN.x, posN.y, sf::Color::Cyan);
			}
			else if (tabNeighbor[j].GetCost() > 1)
			{
				sf::Vector2f posN = tabNeighbor[j].GetNeighbor()->GetData()->getPosition();
				Debug::DrawLine(pos.x, pos.y, posN.x, posN.y, sf::Color::Red);
			}
			else if (tabNeighbor[j].GetCost() == 1)
			{
				sf::Vector2f posN = tabNeighbor[j].GetNeighbor()->GetData()->getPosition();
				Debug::DrawLine(pos.x, pos.y, posN.x, posN.y, sf::Color::Magenta);
			}
			else if (tabNeighbor[j].GetCost() < 1)
			{
				sf::Vector2f posN = tabNeighbor[j].GetNeighbor()->GetData()->getPosition();
				Debug::DrawLine(pos.x, pos.y, posN.x, posN.y, sf::Color::Blue);
			}
		}
	}
}

void MainScene::ResetNode()
{
	if (mRestartNode)
	{
		static bool biTimeReset = false;
		if (biTimeReset == false)
		{
			biTimeReset = true;
			GetGrid()->InitNodeNeighbor();
		}
		else
		{
			mRestartNode = false;
			biTimeReset = false;
		}
	}
}

void MainScene::DisplayUI()
{
	sf::Vector2f txtPos = mView.getCenter();
	sf::Vector2f fpsPos = mView.getCenter();

	sf::Vector2f screenSize = mView.getSize();

	txtPos.x -= screenSize.x * 0.48f;
	txtPos.y -= screenSize.y * 0.48f;

	fpsPos.x -= screenSize.x * 0.48f;
	fpsPos.y -= screenSize.y * 0.40f;

	// The text will appear with the same size even when you zoom in or out
	float dynamicTxtSize = screenSize.x / (float)WINDOW_WIDTH; // Ratio

	Debug::DrawText(txtPos.x, txtPos.y, "Grid Config : " + std::to_string(GridConfigs::GetConfigNumber()), 30 * dynamicTxtSize, sf::Color::Green);



	float dt = GetDeltaTime();
	static int fps = 1.f / dt;
	static float refreshTime = 1.f;

	if (refreshTime < 0)
	{
		refreshTime = 1.f;
		fps = 1.f / dt;
	}
	else
		refreshTime -= dt;

	Debug::DrawText(fpsPos.x, fpsPos.y, "FPS : " + std::to_string(fps), 30 * dynamicTxtSize, sf::Color::Green);
}

void MainScene::SwitchGridFile(int val)
{
	CleanEntities();

	GridConfigs::ChangeConfig(val);

	mpGrid->InitTab(GridConfigs::GetConfigString());
}

void MainScene::SetCellObstacle(sf::Vector2f pos, bool state)
{
	sf::Vector2f fixedPos = { 0, 0 };

	Cell* nearest = GetNearestCell(pos, GetGrid()->GetAllCells());

	if (nearest != nullptr)
	{
		if (nearest->HasAgent() == false)
		{
			nearest->SetIsObstacle(state);
		}
	}
}


