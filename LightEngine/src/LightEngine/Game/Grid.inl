#include "Grid.h"
#include "../GameManager.h"
#include "../Scene.h"
#include "MainScene.h"
#include "../Debug.h"

#include <iostream>
#include <fstream>
#include <string>

template<typename T>
void Grid<T>::Start()
{
	SetCellSize(100);
	pCurrentScene = GameManager::Get()->GetScene<MainScene>();

	if (pCurrentScene)
	{
		//std::string strGrid = GetStringFromTxt("grid.txt");
		//InitTab(strGrid);
		InitTab(std::string());
	}
}

template<typename T>
void Grid<T>::InitTab(std::string strGrid)
{
	int rows = pCurrentScene->GetGridRows();
	int cols = pCurrentScene->GetGridCols();

	mAllCells = *EraseTab<T>(&mAllCells);
	mAllNodes = *EraseTab<Node<T>>(&mAllNodes);
	CreateTab(rows, cols, strGrid);
	InitNodeNeighbor(rows, cols);
	pCurrentScene->GetView().setCenter(GetPositionToView(rows, cols, 0.5f, mCellSize));
}

template<typename T>
void Grid<T>::CreateTab(int rows, int cols, std::string strGrid)
{
	int count = 0;
	for (int r = 0; r < rows; r++)
	{
		std::vector<T*> current;
		for (int c = 0; c < cols; c++)
		{
			T* cell = new T();
			//sf::Vector2f pos = sf::Vector2f(r * mCellSize, c * mCellSize);
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

			Node<T>* node = new Node<T>();
			bool visited = false;
			Node<T>* callMe = nullptr;
			std::vector<Node<T>*> neighbor = std::vector<Node<T>*>();
			int disStart = 0;
			int disEnd = 0;
			node->SetAll(cell, visited, callMe, neighbor, disStart, disEnd);
			mAllNodes.push_back(node);
		}
		mAllCells.push_back(current);
	}
}

template<typename T>
void Grid<T>::InitNodeNeighbor(int rows, int cols)
{
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			Node<T>* node = Node<T>::GetNodeInTab(r, c, rows, &mAllNodes);

			if (node->GetData()->GetObstacle() == true)
			{
				node->SetNeighbor(std::vector<Node<T>*>());
			}
			else
			{
				std::vector<Node<T>*> neighbor = std::vector<Node<T>*>();

				if (c != 0)
				{
					//�
					Node<T>* nodeNeighbor = Node<T>::GetNodeInTab(r, c - 1, rows, &mAllNodes);
					if (nodeNeighbor->GetData()->GetObstacle() == false)
					{
						neighbor.push_back(nodeNeighbor);
					}
				}
				if (r != 0)
				{
					//<-
					Node<T>* nodeNeighbor = Node<T>::GetNodeInTab(r - 1, c, rows, &mAllNodes);
					if (nodeNeighbor->GetData()->GetObstacle() == false)
					{
						neighbor.push_back(nodeNeighbor);
					}
				}
				if (r != rows - 1)
				{
					//->
					Node<T>* nodeNeighbor = Node<T>::GetNodeInTab(r + 1, c, rows, &mAllNodes);
					if (nodeNeighbor->GetData()->GetObstacle() == false)
					{
						neighbor.push_back(nodeNeighbor);
					}
				}
				if (c != cols - 1)
				{
					//!
					Node<T>* nodeNeighbor = Node<T>::GetNodeInTab(r, c + 1, rows, &mAllNodes);
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

template<typename T>
inline void Grid<T>::SaveGrid(std::string fileName)
{
}

template<typename T>
inline std::string Grid<T>::GetStringFromTxt(std::string fileName)
{
	std::string txtOutput;
	std::ifstream file("../../../res/" + fileName);

	if (file.is_open() == false)
		return;

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

template<typename T>
void Grid<T>::Update()
{
	UpdateCellsStatut();
	DrawGrid();
}

template<typename T>
inline void Grid<T>::UpdateCellsStatut()
{
	for (auto row : mAllCells)
	{
		for (auto cell : row)
		{
			cell->CheckStatus(mCellSize);
		}
	}
}

template<typename T>
inline void Grid<T>::DrawGrid()
{
	DrawColorCell();
	pCurrentScene->GetView().setCenter(GetPositionToView(pCurrentScene->GetGridRows(), pCurrentScene->GetGridCols(), 0.5f, mCellSize));
	DrawLineGrid();
}

template<typename T>
inline void Grid<T>::DrawColorCell()
{
	int rows = 0;
	int cols = 0;

	for (auto row : mAllCells)
	{
		for (auto cell : row)
		{
			cell->setPosition(rows * mCellSize, cols * mCellSize);

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

template<typename T>
inline void Grid<T>::DrawLineGrid()
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

template<typename T>
inline sf::Vector2f Grid<T>::GetPositionToView(int rows, int cols, float coef, int size)
{
	return sf::Vector2f((int)((float)rows * coef) * mCellSize, (int)((float)cols * coef) * mCellSize);
}