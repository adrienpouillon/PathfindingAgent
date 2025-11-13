#include "Grid.h"
#include "../GameManager.h"
#include "../Scene.h"
#include "MainScene.h"
#include "../Debug.h"
#include "Functions.h"
#include "Cell.h"

#include <iostream>
#include <fstream>
#include <string>

void Grid::Start()
{
	SetCellSize(100);
	pCurrentScene = GameManager::Get()->GetScene<MainScene>();

	if (pCurrentScene)
	{
		int rows = pCurrentScene->GetGridRows();
		int cols = pCurrentScene->GetGridCols();

		InitTab(rows, cols);
	}
}

void Grid::InitTab(int _rows, int _cols, std::string strGrid)
{
	CleanGrid();
	CreateTab(_rows, _cols, strGrid);
	InitNodeNeighbor();
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

void Grid::CreateTab(int _rows, int _cols, std::string strGrid)
{
	int rows = _rows;
	int cols = _cols;

	int count = 0;
	for (int r = 0; r < rows; r++)
	{
		std::vector<Cell*> current;

		for (int c = 0; c < cols; c++)
		{
			Cell* cell = new Cell();
			sf::Vector2f pos = sf::Vector2f(c * mCellSize, r * mCellSize);
			cell->SetAll(pos, false);

			if (strGrid.size() > 0)
			{
				if (strGrid[count] == 'X')
				{
					cell->SetObstacle(true);
				}
				count++;
			}

			current.push_back(cell);

			Node<Cell>* node = new Node<Cell>();
			bool visited = false;
			Node<Cell>* callMe = nullptr;
			std::vector<Node<Cell>*> neighbors = std::vector<Node<Cell>*>();
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

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			Node<Cell>* node = GetNodeInTab(r, c, rows, &mAllNodes);

			if (node->GetData()->GetObstacle() == true)
			{
				node->SetNeighbor(std::vector<Node<Cell>*>());
			}
			else
			{
				std::vector<Node<Cell>*> neighbor = std::vector<Node<Cell>*>();

				if (c != 0)
				{
					//î
					Node<Cell>* nodeNeighbor = GetNodeInTab(r, c - 1, rows, &mAllNodes);
					if (nodeNeighbor->GetData()->GetObstacle() == false)
					{
						neighbor.push_back(nodeNeighbor);
					}
				}
				if (r != 0)
				{
					//<-
					Node<Cell>* nodeNeighbor = GetNodeInTab(r - 1, c, rows, &mAllNodes);
					if (nodeNeighbor->GetData()->GetObstacle() == false)
					{
						neighbor.push_back(nodeNeighbor);
					}
				}
				if (r != rows - 1)
				{
					//->
					Node<Cell>* nodeNeighbor = GetNodeInTab(r + 1, c, rows, &mAllNodes);
					if (nodeNeighbor->GetData()->GetObstacle() == false)
					{
						neighbor.push_back(nodeNeighbor);
					}
				}
				if (c != cols - 1)
				{
					//!
					Node<Cell>* nodeNeighbor = GetNodeInTab(r, c + 1, rows, &mAllNodes);
					if (nodeNeighbor->GetData()->GetObstacle() == false)
					{
						neighbor.push_back(nodeNeighbor);
					}
				}

				node->SetNeighbor(neighbor);
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

	for (auto& row : mAllCells)
	{
		for (auto cell : row)
		{
			if (cell->GetObstacle())
			{
				file << "X ";
			}
			else
			{
				file << "O ";
			}
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
	pCurrentScene->GetView().setCenter(GetPositionToView(pCurrentScene->GetGridRows(), pCurrentScene->GetGridCols(), 0.5f, mCellSize));
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

			if (cell->GetAgent() == true)
				indicator = sf::Color(255, 0, 0, 100);

			if (cell->GetObstacle() == true)
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
	int lenghtCol = mAllCells.size();
	int lenghtRow = mAllCells[0].size();

	for (int col = 0; col < lenghtCol + 1; col++)
	{
		sf::Vector2f startPos = mAllCells[0][0]->getPosition();
		sf::Vector2f endPos = mAllCells[lenghtCol - 1][lenghtRow - 1]->getPosition();

		sf::Vector2f p1 = { startPos.x + mCellSize * col - mCellSize * 0.5f, startPos.y - mCellSize * 0.5f };
		sf::Vector2f p2 = { startPos.x + mCellSize * col - mCellSize * 0.5f, endPos.y + mCellSize * 0.5f };

		Debug::DrawLine(p1.x, p1.y, p2.x, p2.y, sf::Color::White);
	}

	for (int row = 0; row < lenghtRow + 1; row++)
	{
		sf::Vector2f startPos = mAllCells[0][0]->getPosition();
		sf::Vector2f endPos = mAllCells[lenghtCol - 1][lenghtRow - 1]->getPosition();

		sf::Vector2f p1 = { startPos.x - mCellSize * 0.5f, startPos.y + mCellSize * row - mCellSize * 0.5f };
		sf::Vector2f p2 = { endPos.x + mCellSize * 0.5f, startPos.y + mCellSize * row - mCellSize * 0.5f };

		Debug::DrawLine(p1.x, p1.y, p2.x, p2.y, sf::Color::White);
	}
}

sf::Vector2f Grid::GetPositionToView(int rows, int cols, float coef, int size)
{
	return sf::Vector2f((int)((float)rows * coef) * mCellSize, (int)((float)cols * coef) * mCellSize);
}