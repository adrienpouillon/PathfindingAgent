#include "Grid.h"
#include "../GameManager.h"
#include "../Scene.h"
#include "MainScene.h"
#include "../Debug.h"

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
	for (auto& row : mAllCells)
	{
		for (auto& cell : row)
		{
			sf::Vector2f pos = cell.getPosition();

			Debug::DrawCircle(pos.x, pos.y, 5, sf::Color::Red);
		}
	}
}