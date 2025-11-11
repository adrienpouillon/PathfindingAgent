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

		for (int r = 0; r < rows; r++)
		{
			std::vector<Cell> current;

			for (int c = 0; c < cols; c++)
			{
				Cell cell;
				cell.SetSize(mCellSize);

				cell.setPosition(r * mCellSize, c * mCellSize);

				current.push_back(cell);
			}

			mAllCells.push_back(current);
		}

		for (auto& row : mAllCells)
		{
			for (auto& cell : row)
			{
				Node<T> node;
				node.SetData(&cell);

				mAllNodes.push_back(node);
			}
		}

		pScene->GetView().setCenter((int)((float)rows * 0.5f) * mCellSize, (int)((float)cols * 0.5f) * mCellSize);
	}
}

template<typename T>
void Grid<T>::Update()
{
	UpdateCellsStatut();
	DrawGrid();
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
