#include "Utils.h"
#include "Game/Grid.h"
#include "Game/Node.h"

namespace Utils
{
	template<typename T>
	inline T* GetCell(sf::Vector2f pos, Grid<T>* grid)
	{
		sf::Vector2f index = GetCellInPos(pos, grid->GetCellSize());
		return grid->GetAllCells()[index.x][index.y];
	}

	template<typename T>
	inline Node<T>* GetNode(sf::Vector2f pos, Grid<T>* grid)
	{
		sf::Vector2f index = GetCellInPos(pos, grid->GetCellSize());
		std::vector<Node<T>*> allNodes = grid->GetAllNodes();
		return Node<T>::GetNodeInTab(index.x, index.y, grid->GetAllCells().size(), &allNodes);
	}



}



