#include "Grid.h"

#include "../GameManager.h"
#include "../Scene.h"
#include "../Debug.h"

#include "MainScene.h"
#include "Functions.h"
#include "Cell.h"
#include "Node.h"

#include <iostream>
#include <fstream>
#include <string>

void Grid::Start()
{
	mGuardsDataFromTxt.clear();
	mCivilDataFromTxt.clear();
	mAssassinDataFromTxt.clear();
	pCurrentScene = GameManager::Get()->GetScene<MainScene>();
}

void Grid::InitTab(int _rows, int _cols, std::string strGrid)
{
	CleanGrid();
	CreateTab(_rows, _cols, strGrid);
	InitNodeNeighbor(_rows, _cols);

	AddAgentsFromTxt();

	pCurrentScene->GetView().setCenter(GetPositionToView(_rows, _cols, 0.5f, mCellSize));
}

void Grid::InitTab(std::string fileName)
{
	std::string strGrid = GetStringFromTxt(fileName);

	if (strGrid.empty())
	{
		std::cout << "error : file can't be found or read !\n";
		return;
	}

	InitTab(pCurrentScene->GetGridRows(), pCurrentScene->GetGridCols(), strGrid);
}

void Grid::CreateTab(int _rows, int _cols, std::string strGrid = "")
{
	int count = 0;
	for (int r = 0; r < _rows; r++)
	{
		std::vector<Cell*> current;

		for (int c = 0; c < _cols; c++)
		{
			Cell* cell = new Cell();
			sf::Vector2f pos = sf::Vector2f(c * mCellSize, r * mCellSize);
			cell->SetAll(pos, false, r, c);

			if (strGrid.size() > 0)
			{
				if (strGrid[count] == 'X')
				{
					cell->SetIsObstacle(true);
				}
				else if(strGrid[count] == 'G')
				{
					mGuardsDataFromTxt.push_back(AgentsData(sf::Vector2f(c * mCellSize, r * mCellSize), 100.f, 25.f, sf::Color::Cyan, sf::Color::Blue));
				}
				else if (strGrid[count] == 'C')
				{
					mCivilDataFromTxt.push_back(AgentsData(sf::Vector2f(c * mCellSize, r * mCellSize), 75.f, 25.f, sf::Color::Red + sf::Color::Green, sf::Color::Red));
				}
				else if (strGrid[count] == 'A')
				{
					mAssassinDataFromTxt.push_back(AgentsData(sf::Vector2f(c * mCellSize, r * mCellSize), 100.f, 25.f, sf::Color::Red, sf::Color::Magenta));
				}

				count++;
			}

			current.push_back(cell);

			Node<Cell>* node = new Node<Cell>();
			bool visited = false;
			Node<Cell>* callMe = nullptr;
			std::vector<NeighborsCost<Cell>> neighbors = std::vector<NeighborsCost<Cell>>();
			int disStart = 0;
			int disEnd = 0;
			node->SetAll(cell, visited, callMe, neighbors, disStart, disEnd);
			mAllNodes.push_back(node);
		}

		mAllCells.push_back(current);
	}
}

void Grid::InitNodeNeighbor()
{
	int rows = pCurrentScene->GetGridRows();
	int cols = pCurrentScene->GetGridCols();

	InitNodeNeighbor(rows, cols);
}

void Grid::AddAgentsFromTxt()
{
	for (AgentsData& ad : mGuardsDataFromTxt)
	{
		pCurrentScene->CreateGuard(ad.pos, ad.speed, ad.radius, ad.classicColor, ad.roamColor);
	}
	for (AgentsData& ad : mCivilDataFromTxt)
	{
		pCurrentScene->CreateCivil(ad.pos, ad.speed, ad.radius, ad.classicColor, ad.roamColor);
	}
	for (AgentsData& ad : mAssassinDataFromTxt)
	{
		pCurrentScene->CreateAssassin(ad.pos, ad.speed, ad.radius, ad.classicColor, ad.roamColor);
	}

	mGuardsDataFromTxt.clear();
	mCivilDataFromTxt.clear();
	mAssassinDataFromTxt.clear();

}

void Grid::InitNodeNeighbor(int _rows, int _cols)
{
	for (int r = 0; r < _rows; r++)
	{
		for (int c = 0; c < _cols; c++)
		{
			Node<Cell>* node = GetNodeInTab(r, c, _cols, &mAllNodes);

			std::vector<NeighborsCost<Cell>>& tabNodeCost = node->GetNeighborsCost();
			tabNodeCost.clear();

			if (node->GetData()->IsObstacle() == true)
			{
				continue;
			}

			bool right = false;
			bool left = false;
			bool up = false;
			bool down = false;

			float r1 = 1;
			float r2 = RACINE;

			if (c != 0)
			{
				//i
				Node<Cell>* nodeNeighbor = GetNodeInTab(r, c - 1, _cols, &mAllNodes);
				if (nodeNeighbor->GetData()->IsObstacle() == false)
				{
					tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r1));
					up = true;
				}
			}
			if (r != 0)
			{
				//<-
				Node<Cell>* nodeNeighbor = GetNodeInTab(r - 1, c, _cols, &mAllNodes);
				if (nodeNeighbor->GetData()->IsObstacle() == false)
				{
					tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r1));
					left = true;
				}
			}
			if (r != _rows - 1)
			{
				//->
				Node<Cell>* nodeNeighbor = GetNodeInTab(r + 1, c, _cols, &mAllNodes);
				if (nodeNeighbor->GetData()->IsObstacle() == false)
				{
					tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r1));
					right = true;
				}
			}
			if (c != _cols - 1)
			{
				//!
				Node<Cell>* nodeNeighbor = GetNodeInTab(r, c + 1, _cols, &mAllNodes);
				if (nodeNeighbor->GetData()->IsObstacle() == false)
				{
					tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r1));
					down = true;
				}
			}

			if (up)
			{
				if (left)
				{
					if (r != 0 && c != 0)
					{
						Node<Cell>* nodeNeighbor = GetNodeInTab(r - 1, c - 1, _cols, &mAllNodes);
						if (nodeNeighbor->GetData()->IsObstacle() == false)
						{
							tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r2));
						}
					}
				}

				if (right)
				{
					if (r != _rows - 1 && c != 0)
					{
						Node<Cell>* nodeNeighbor = GetNodeInTab(r + 1, c - 1, _cols, &mAllNodes);
						if (nodeNeighbor->GetData()->IsObstacle() == false)
						{
							tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r2));
						}
					}
				}
			}

			if (down)
			{
				if (left)
				{
					if (r != 0 && c != _cols - 1)
					{
						Node<Cell>* nodeNeighbor = GetNodeInTab(r - 1, c + 1, _cols, &mAllNodes);
						if (nodeNeighbor->GetData()->IsObstacle() == false)
						{
							tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r2));
						}
					}
				}

				if (right)
				{
					if (r != _rows - 1 && c != _cols - 1)
					{
						Node<Cell>* nodeNeighbor = GetNodeInTab(r + 1, c + 1, _cols, &mAllNodes);
						if (nodeNeighbor->GetData()->IsObstacle() == false)
						{
							tabNodeCost.push_back(NeighborsCost<Cell>(nodeNeighbor, r2));
						}
					}
				}
			}


		}
	}
}

void Grid::CleanGrid()
{
	for (auto it = mAllCells.begin(); it != mAllCells.end(); ++it)
	{
		for (auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			delete* it2;
		}
	}

	for (auto& row : mAllCells)
	{
		row.clear();
	}
	mAllCells.clear();


	for (auto it = mAllNodes.begin(); it != mAllNodes.end(); ++it)
	{
		delete* it;
	}
	mAllNodes.clear();
}

void Grid::SaveGrid(std::string fileName)
{
	std::ofstream file("../../../res/" + fileName, std::ios::out);

	if (file.is_open() == false)
		return;

	for (int r = 0; r < pCurrentScene->GetGridRows(); r++)
	{
		for (int c = 0; c < pCurrentScene->GetGridCols(); c++)
		{
			Cell* currentCell = mAllCells[r][c];

			std::string toAdd = "O";

			if (currentCell->IsObstacle())
			{
				toAdd = "X";
			}
			
			if (currentCell->HasAgent())
			{
				if (Entity* e = GetNearestEntity(currentCell->getPosition()))
				{
					if (e->IsType(Tag::AGENT))
					{
						Cell* nearest = GetNearestCell(e->GetPosition(), pCurrentScene->GetGrid()->GetAllCells());

						if (currentCell == nearest)
						{
							if (e->IsTag(Tag::GUARD))
							{
								toAdd = "G";
							}
							else if(e->IsTag(Tag::CIVIL))
							{
								toAdd = "C";
							}
							else if (e->IsTag(Tag::ASSASSIN))
							{
								toAdd = "A";
							}
						}
					}
				}
			}

			file << toAdd + " ";
		}

		file << std::endl;
	}

	file.close();
}

std::string Grid::GetStringFromTxt(std::string fileName)
{
	std::string txtOutput;
	std::ifstream file("../../../res/" + fileName);

	if (file.is_open() == false)
	{
		std::cerr << fileName + " can't be open !" << std::endl;
		return "";
	}

	std::string line;
	int rowsCount = 0;

	while (std::getline(file, line))
	{
		for (auto it = line.begin(); it != line.end();)
		{
			if (*it == ' ')
			{
				it = line.erase(it);
				continue;
			}
			else
			{
				++it;
			}
		}

		txtOutput += line;

		pCurrentScene->SetGridCols(line.size());

		rowsCount++;
	}

	pCurrentScene->SetGridRows(rowsCount);
	file.close();

	return txtOutput;
}

void Grid::Update()
{
	UpdateCellsStatut();
	DrawGrid();
}

void Grid::UpdateCellsStatut()
{
	for (auto& row : mAllCells)
	{
		for (auto cell : row)
		{
			cell->CheckStatus(mCellSize);
		}
	}
}

void Grid::DrawGrid()
{
	DrawColorCell();
	DrawLineGrid();
}

void Grid::DrawColorCell()
{
	int rows = 0;
	int cols = 0;

	for (auto& row : mAllCells)
	{
		for (auto cell : row)
		{
			sf::Color indicator = sf::Color::Transparent;

			if (cell->HasAgent() == true)
				indicator = sf::Color(255, 0, 0, 100);

			if (cell->IsObstacle() == true)
			{
				indicator = sf::Color(255, 255, 255, 255);
			}

			sf::Vector2f pos = cell->getPosition();

			Debug::DrawRectangle(pos.x, pos.y, mCellSize, mCellSize, indicator);

			cols++;
		}

		rows++;
		cols = 0;
	}
}

void Grid::DrawLineGrid()
{
	int lenghtCol = mAllCells[0].size();
	int lenghtRow = mAllCells.size();

	for (int col = 0; col < lenghtCol + 1; col++)
	{
		sf::Vector2f startPos = mAllCells.front().front()->getPosition();
		sf::Vector2f endPos = mAllCells.back().back()->getPosition();

		sf::Vector2f p1 = { startPos.x + mCellSize * col - mCellSize * 0.5f, startPos.y - mCellSize * 0.5f };
		sf::Vector2f p2 = { startPos.x + mCellSize * col - mCellSize * 0.5f, endPos.y + mCellSize * 0.5f };

		Debug::DrawLine(p1.x, p1.y, p2.x, p2.y, sf::Color::White);
	}

	for (int row = 0; row < lenghtRow + 1; row++)
	{
		sf::Vector2f startPos = mAllCells.front().front()->getPosition();
		sf::Vector2f endPos = mAllCells.back().back()->getPosition();

		sf::Vector2f p1 = { startPos.x - mCellSize * 0.5f, startPos.y + mCellSize * row - mCellSize * 0.5f };
		sf::Vector2f p2 = { endPos.x + mCellSize * 0.5f, startPos.y + mCellSize * row - mCellSize * 0.5f };

		Debug::DrawLine(p1.x, p1.y, p2.x, p2.y, sf::Color::White);
	}
}

sf::Vector2f Grid::GetPositionToView(int rows, int cols, float coef, int size)
{
	return sf::Vector2f((int)((float)rows * coef) * mCellSize, (int)((float)cols * coef) * mCellSize);
}

void Grid::SetParameters(int cellSize, bool usingTxt)
{
	mCellSize = cellSize;

	int rows = pCurrentScene->GetGridRows();
	int cols = pCurrentScene->GetGridCols();

	if (usingTxt)
		InitTab(GridConfigs::GetConfigString());
	else
		InitTab(rows, cols);
}