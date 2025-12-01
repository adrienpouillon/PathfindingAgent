#include "pch.h"
#include "MainScene.h"
#include "Grid.h"
#include "Agent.h"
#include "Civil.h"
#include "Guard.h"
#include "Assassin.h"
#include "Cursor.h"
#include "Functions.h"
#include <iostream>


void MainScene::OnInitialize()
{
	mSelectedEntity = nullptr;
	mIsSaving = false;
	mRestartNode = false;
	mDrawDebug = 0;
	mUseCoin = false;

	mpCam = GameManager::Get()->GetCamera();
	mpCam->SetRotation({ 90.f, 0.f, 0.f });
	mpCam->SetPosition({ 0.f, 250.f, 0.f });

	mpGrid = new Grid();
	mpGrid->SetParameters(50, true);

	float cellSize = mpGrid->GetCellSize();

	gce::Vector3f32 cursorScale = { cellSize, cellSize, cellSize };
	cursorScale.x *= 0.9f;
	cursorScale.y = 3.f;
	cursorScale.z *= 0.9f;

	mpCursor = CreateEntity<Cursor>(new Cube(), cursorScale, gce::Vector3f32(0.5f, 0.5f, 0.5f));
	mpCursor->GetGeo()->SetPosition({ 0, 0, 0 });

	srand(time(nullptr));
}

void MainScene::OnUpdate()
{
	DisplayUI();
	InputManager();

	mpGrid->Update();

	DrawSelectedEntity();

	DebugDrawPath();
}

void MainScene::InputManager()
{
	if (mpCursor == nullptr)
		return;
	
	if (mpCursor->ToDestroy())
		return;

	gce::Vector3f32 cursorPos = mpCursor->GetPosition();

	int actionExecute = 0;

	MousePressedInputs(cursorPos, actionExecute);
	MouseInputs(cursorPos, actionExecute);
	KeyPressedInputs(cursorPos, actionExecute);
	KeyInputs(cursorPos, actionExecute);

	ResetNode();
}

void MainScene::MousePressedInputs(gce::Vector3f32 cursorPos, int& actionExecute)
{
	if (GetButtonDown(Mouse::LEFT))
	{
		if (GetKey(Keyboard::A) && actionExecute < 1)
		{
			//ajouter un agent

			Cell* nearest = GetNearestCell(cursorPos, mpGrid->GetAllCells());

			float cellSize = mpGrid->GetCellSize();
			float agentSize = cellSize * 0.5f;

			if (nearest->HasAgent() == false && nearest->IsObstacle() == false)
			{
				gce::Vector3f32 pos = nearest->GetPosition();
				pos.y += (agentSize + nearest->GetRealHeight()) * 0.5f;

				nearest->SetHasAgent(true);

				Agent* a = CreateGuard(pos, 100.f, agentSize, gce::Vector3f32(0.f, 1.f, 1.f), gce::Vector3f32(0.f, 0.f, 1.f));
				a->SetConfigHeight(nearest->GetConfigHeight());
				actionExecute++;
			}
		}

		if (GetKey(Keyboard::C) && actionExecute < 1)
		{
			//ajouter une piece
			Cell* currentCell = GetNearestCell(cursorPos, mpGrid->GetAllCells());
			Entity* e = GetNearestAgent(cursorPos);
			if (e == nullptr && currentCell->HasCoin() == false)
			{
				currentCell->SetHasCoin(true);
				actionExecute++;
			}
		}

		if (actionExecute < 1)
		{
			Cell* nearest = GetNearestCell(cursorPos, mpGrid->GetAllCells());
			if (nearest->HasAgent() == false && mSelectedEntity != nullptr)
			{
				if (Agent* a = dynamic_cast<Agent*>(mSelectedEntity))
				{
					a->GoToCell(cursorPos, mpGrid);
					std::cout << "Moving !\n";
					actionExecute++;
				}
			}
		}
		if (actionExecute < 1)
		{
			Entity* clicOnEntity = GetNearestAgent(cursorPos);
			if (clicOnEntity != nullptr)
			{
				if (clicOnEntity == mSelectedEntity)
				{
					mSelectedEntity->ResetGeometryColor();
					mSelectedEntity = nullptr;
					actionExecute++;
				}
				else
				{
					if (mSelectedEntity != nullptr)
					{
						mSelectedEntity->ResetGeometryColor();
					}

					mSelectedEntity = clicOnEntity;
					actionExecute++;
				}
			}
		}
	}

	if (GetButtonDown(Mouse::MIDDLE))
	{
		if (GetKey(Keyboard::A) && actionExecute < 1)
		{
			//ajouter un agent

			Cell* nearest = GetNearestCell(cursorPos, mpGrid->GetAllCells());

			float cellSize = mpGrid->GetCellSize();
			float agentSize = cellSize * 0.5f;

			if (nearest->HasAgent() == false && nearest->IsObstacle() == false)
			{
				gce::Vector3f32 pos = nearest->GetPosition();
				pos.y += (agentSize + nearest->GetRealHeight()) * 0.5f;

				nearest->SetHasAgent(true);

				CreateCivil(pos, 100.f, agentSize, gce::Vector3f32(1.f, 0.5f, 0.f), gce::Vector3f32(1.f, 0.f, 0.f));
				actionExecute++;
			}
		}

		if(actionExecute < 1)
		{
			Cell* currentCell = GetNearestCell(cursorPos, mpGrid->GetAllCells());
			Entity* e = GetNearestAgent(cursorPos);
			if (e == nullptr)
			{
				int currentHeight = currentCell->GetConfigHeight();
				currentCell->ChangeHeight(currentHeight + 1);
				mRestartNode = true;
				actionExecute++;
			}
		}
	}

	if (GetButtonDown(Mouse::RIGHT))
	{
		if (GetKey(Keyboard::A) && actionExecute < 1)
		{
			//ajouter un agent

			Cell* nearest = GetNearestCell(cursorPos, mpGrid->GetAllCells());

			float cellSize = mpGrid->GetCellSize();
			float agentSize = cellSize * 0.5f;

			if (nearest->HasAgent() == false && nearest->IsObstacle() == false)
			{
				gce::Vector3f32 pos = nearest->GetPosition();
				pos.y += (agentSize + nearest->GetRealHeight()) * 0.5f;

				nearest->SetHasAgent(true);

				CreateAssassin(pos, 100.f, agentSize, gce::Vector3f32(1.f, 0.f, 0.f), gce::Vector3f32(1.f, 0.f, 1.f));
				actionExecute++;
			}
		}

		if (actionExecute < 1)
		{
			if (Entity* e = GetNearestAgent(cursorPos))
			{
				//mRestartNode = true;
				mSelectedEntity = nullptr;
				e->Destroy();
				actionExecute++;
			}
		}
	}
}

void MainScene::KeyPressedInputs(gce::Vector3f32 cursorPos, int& actionExecute)
{
	HandleResizingGrid();
	HandleChangingGridConfig();

	if (GetKeyDown(Keyboard::P) && actionExecute < 1)
	{
		if (Agent* a = dynamic_cast<Agent*>(mSelectedEntity))
		{
			a->SetRoam(!a->GetRoam());
			actionExecute++;
		}
	}

	if (GetKeyDown(Keyboard::R) && actionExecute < 1)
	{
		ResetCamSettings();
		actionExecute++;
	}

	if (GetKeyDown(Keyboard::H) && actionExecute < 1)
	{
		CleanEntities();
		actionExecute++;
	}

	if (GetKeyDown(Keyboard::G) && actionExecute < 1)
	{
		mRestartNode = true;
		CleanWalls();
		actionExecute++;
	}

	if (GetKeyDown(Keyboard::F) && actionExecute < 1)
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

	if (GetKeyDown(Keyboard::K) && actionExecute < 1)
	{
		mUseCoin = !mUseCoin;
		actionExecute++;
	}
}

void MainScene::MouseInputs(gce::Vector3f32 cursorPos, int& actionExecute)
{

}

void MainScene::KeyInputs(gce::Vector3f32 cursorPos, int& actionExecute)
{
	HandleGridSave();
	HandleCameraTransformations();
}

void MainScene::HandleCameraTransformations()
{
	HandleCamMove();
	HandleCamZoom();
}

void MainScene::HandleResizingGrid()
{
	if (GetKeyDown(Keyboard::NUMPAD_SUBTRACT))
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
		mpCursor->ResetFollowGridCoords();

		mpGrid->InitTab(mGridRows, mGridCols);
	}
	else if (GetKeyDown(Keyboard::NUMPAD_ADD))
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
		mpCursor->ResetFollowGridCoords();

		mpGrid->InitTab(mGridRows, mGridCols);
	}
}

void MainScene::HandleCamMove()
{
	float dt = GetDeltaTime();

	float finalSpeedDt = CamSpeed * dt;

	gce::Vector3f32 dir = { 0, 0, 0 };

	dir.x = GetKey(Keyboard::NUMPAD6) - GetKey(Keyboard::NUMPAD4);
	dir.z = GetKey(Keyboard::NUMPAD8) - GetKey(Keyboard::NUMPAD5);

	dir = dir.Normalize();

	GameManager::Get()->GetCamera()->Translate(dir * finalSpeedDt);

	gce::Vector3f32 dir2 = { 0, 0, 0 };

	dir2.x = GetKey(Keyboard::DOWN) - GetKey(Keyboard::UP);
	dir2.y = GetKey(Keyboard::RIGHT) - GetKey(Keyboard::LEFT);

	dir2 = dir2.Normalize();

	GameManager::Get()->GetCamera()->Rotate(dir2 * dt);
}

void MainScene::HandleGridSave()
{
	if (GetKey(Keyboard::LCTRL) == false && GetKey(Keyboard::S) == false)
	{
		mIsSaving = false;
	}

	if (mIsSaving == true)
	{
		gce::Vector2f32 center = { (float)WINDOW_WIDTH * 0.4f, (float)WINDOW_HEIGHT * 0.5f};

		Debug::DrawText(center.x, center.y, L"Grid Saved !", 0.5f, 0.5f, 100, gce::Color::Blue);
		return;
	}

	if (GetKey(Keyboard::LCTRL) && GetKey(Keyboard::S))
	{
		std::cout << "SAVED !\n";

		mpGrid->SaveGrid(GridConfigs::GetConfigString());

		mIsSaving = true;
	}
}

void MainScene::HandleChangingGridConfig()
{
	if (GetKeyDown(Keyboard::F1))
	{
		SwitchGridFile(-1);
	}

	if (GetKeyDown(Keyboard::F2))
	{
		SwitchGridFile(+1);
	}
}

Agent* MainScene::CreateAgent(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor)
{
	Agent* newEntity = CreateEntity<Agent>(new Sphere(), gce::Vector3f32(size, size, size), gce::Vector3f32(1.f, 1.f, 1.f));
	newEntity->SetRoam(false);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y, pos.z);

	return newEntity;
}

Civil* MainScene::CreateCivil(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor)
{
	Civil* newEntity = CreateEntity<Civil>(new Sphere(), gce::Vector3f32(size, size, size), gce::Vector3f32(1.f, 1.f, 1.f));

	//agent
	newEntity->SetRoam(false);
	newEntity->SetTag(CIVIL);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y, pos.z);

	return newEntity;
}

Guard* MainScene::CreateGuard(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor)
{
	Guard* newEntity = CreateEntity<Guard>(new Sphere(), gce::Vector3f32(size, size, size), gce::Vector3f32(1.f, 1.f, 1.f));
	newEntity->SetRoam(false);
	newEntity->SetTag(GUARD);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y, pos.z);

	return newEntity;
}

Assassin* MainScene::CreateAssassin(gce::Vector3f32 pos, float speed, int size, gce::Vector3f32 classicColor, gce::Vector3f32 roamColor)
{
	Assassin* newEntity = CreateEntity<Assassin>(new Sphere(), gce::Vector3f32(size, size, size), gce::Vector3f32(1.f, 1.f, 1.f));
	newEntity->SetRoam(false);
	newEntity->SetTag(ASSASSIN);
	newEntity->SetSpeed(speed);
	newEntity->SetRigidBody(false);
	newEntity->SetPathfinding(CreatePathFinding());
	newEntity->SetClassicColor(classicColor);
	newEntity->SetRoamColor(roamColor);

	newEntity->SetPosition(pos.x, pos.y, pos.z);

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
		if(e->IsType(Tag::AGENT))
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
			c->ChangeHeight(1);
		}
	}
}

void MainScene::HandleCamZoom()
{
	float dt = GetDeltaTime();

	float zoom = CamSpeed * dt;

	if (mpCam == nullptr)
		return;

	if (GetKey(Keyboard::NUMPAD7))
	{
		mpCam->Translate({ 0, zoom, 0 });
	}
	else if (GetKey(Keyboard::NUMPAD9))
	{
		mpCam->Translate({ 0, -zoom, 0 });
	}
}

void MainScene::ResetCamSettings()
{
	if (mpGrid != nullptr)
	{
		gce::Vector3f32 centerView = mpGrid->GetPositionToView(mGridRows, mGridCols, 0.5f);

		mpCam->SetPosition(centerView);
		mpCam->SetRotation({ 90.f, 0.f, 0.f });
	}
}

void MainScene::DrawSelectedEntity()
{
	if (mSelectedEntity != nullptr && mSelectedEntity->ToDestroy() == false)
	{
		Geometry* pGeo = mSelectedEntity->GetGeo();

		gce::Vector3f32 color = pGeo->GetDefaultColor();
		gce::Vector3f32 invertedColor = { 1.f - color.x, 1.f - color.y, 1.f - color.z };

		mSelectedEntity->GetGeo()->SetColor(invertedColor);
	}
}

void MainScene::DebugDrawPath()
{
	/*std::vector<Node<Cell>*> tabNode = GetGrid()->GetAllNodes();
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
	}*/
}

void MainScene::ResetNode()
{
	if (mRestartNode == true)
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
	gce::Vector2i32 txtConfigPos = { 20, 75 };
	Debug::DrawText(txtConfigPos.x, txtConfigPos.y, L"Config : " + std::to_wstring(GridConfigs::GetConfigNumber()), 10, gce::Color::Black);

	gce::Vector2i32 txtFPSpos = { 20, 20 };
	float dt = GetDeltaTime();
	static float refreshTime = 1.f;
	static int fps = (int)(1.f / dt);

	if (refreshTime < 0)
	{
		refreshTime = 1.f;
		fps = (int)(1.f / dt);
	}
	else
		refreshTime -= dt;

	Debug::DrawText(txtFPSpos.x, txtFPSpos.y, L"FPS :" + std::to_wstring(fps), 10, gce::Color::Black);
}


void MainScene::SwitchGridFile(int val)
{
	CleanEntities();
	mpCursor->ResetFollowGridCoords();

	GridConfigs::ChangeConfig(val);

	mpGrid->InitTab(GridConfigs::GetConfigString());
}


