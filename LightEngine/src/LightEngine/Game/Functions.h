#pragma once

#include "../Utils.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

class Grid;

class Entity;

template<typename T>
class Node;

template<typename T>
void ResetNodes(Grid* grid);

template<typename T>
/*std::vector<Node<T>*>* */ void FindPath(Node<T>* startNode, Node<T>* endNode, Grid* grid, std::vector<Node<T>*>* allFormerPaths);

template<typename T>
/*std::vector<Node<T>*>* */ void CreatePath(bool isfinish, Node<T>* startNode, Node<T>* endNode, std::vector<Node<T>*>* allFormerPaths);

template<typename T>
T* GetCell(sf::Vector2f pos, Grid* grid);

template<typename T>
Node<T>* GetNode(sf::Vector2f pos, Grid* grid);

template<typename T>
Node<T>* GetNodeInTab(int i, int j, int lenght_i, std::vector<Node<T>*>* allNodes);

template<typename A>
std::vector<A*>* EraseTab(std::vector<A*>* all);

template<typename A>
std::vector<std::vector<A*>>* EraseTab(std::vector<std::vector<A*>>* all);

Entity* GetNearestEntity(sf::Vector2f pos);

template<typename T>
T* GetNearestCell(sf::Vector2f pos, std::vector<std::vector<T*>> allCells);









#include "Functions.inl"





