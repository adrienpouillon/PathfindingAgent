#include "Grid.h"
#include "../GameManager.h"
#include "../Scene.h"
#include "MainScene.h"
#include "../Debug.h"
#include <iostream>

template<typename T>
void Grid<T>::Start()
{
	if (MainScene* pScene = dynamic_cast<MainScene*>(GameManager::Get()->GetScene()))
	{
		int rows = pScene->GetGridRows();
		int cols = pScene->GetGridCols();

		InitTab(rows, cols);
		InitNodeNeighbor(rows, cols);
		pScene->GetView().setCenter((int)((float)rows * 0.5f) * mCellSize, (int)((float)cols * 0.5f) * mCellSize);
	}
}

template<typename T>
void Grid<T>::InitTab(int rows, int cols)
{
	for (int r = 0; r < rows; r++)
	{
		std::vector<T> current;
		for (int c = 0; c < cols; c++)
		{
			T cell;
			cell.SetSize(mCellSize);
			sf::Vector2f pos = sf::Vector2f(r * mCellSize, c * mCellSize);
			cell.SetAll(pos, false);

			current.push_back(cell);

			Node<T> node;
			bool visited = false;
			Node<T>* callMe = nullptr;
			std::vector<Node<T>*> neighbor = std::vector<Node<T>*>();
			int disStart = 0;
			int disEnd = 0;
			node.SetAll(&cell, visited, callMe, neighbor, disStart, disEnd);
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
					//î
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
void Grid<T>::Update()
{
	UpdateCellsStatut();
	DrawGrid();
}

template<typename T>
inline void Grid<T>::UpdateCellsStatut()
{
	for (auto& row : mAllCells)
	{
		for (auto& cell : row)
		{
			cell.CheckStatus();
		}
	}
}

template<typename T>
inline void Grid<T>::DrawGrid()
{
	for (auto& row : mAllCells)
	{
		for (auto& cell : row)
		{
			sf::Color indicator = sf::Color::Transparent;

			if (cell.GetObstacle())
				indicator = sf::Color(100, 100, 100, 0);

			if (cell.GetAgent())
				indicator = sf::Color(255, 0, 0, 100);

			sf::Vector2f pos = cell.getPosition();

			Debug::DrawRectangle(pos.x, pos.y, mCellSize, mCellSize, indicator);
		}
	}

	for (int col = 0; col < mAllCells.size() + 1; col++)
	{
		sf::Vector2f startPos = mAllCells[0][0].getPosition();
		sf::Vector2f endPos = mAllCells[mAllCells.size() - 1][mAllCells[0].size() - 1].getPosition();

		sf::Vector2f p1 = { startPos.x + mCellSize * col - mCellSize * 0.5f, startPos.y - mCellSize * 0.5f };
		sf::Vector2f p2 = { startPos.x + mCellSize * col - mCellSize * 0.5f, endPos.y + mCellSize * 0.5f };

		Debug::DrawLine(p1.x, p1.y, p2.x, p2.y, sf::Color::White);
	}

	for (int row = 0; row < mAllCells[0].size() + 1; row++)
	{
		sf::Vector2f startPos = mAllCells[0][0].getPosition();
		sf::Vector2f endPos = mAllCells[mAllCells.size() - 1][mAllCells[0].size() - 1].getPosition();

		sf::Vector2f p1 = { startPos.x - mCellSize * 0.5f, startPos.y + mCellSize * row - mCellSize * 0.5f };
		sf::Vector2f p2 = { endPos.x + mCellSize * 0.5f, startPos.y + mCellSize * row - mCellSize * 0.5f };

		Debug::DrawLine(p1.x, p1.y, p2.x, p2.y, sf::Color::White);
	}
}
