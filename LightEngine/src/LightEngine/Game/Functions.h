#pragma once

#include "../Utils.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

template<typename T>
class Grid;

template<typename T>
class Node;

template<typename T>
void ResetNodes(Grid<T>* grid);

template<typename T>
void FindPath(Node<T>* startNode, Node<T>* endNode, Grid<T>* grid, std::vector<Node<T>*> allFormerPaths);

template<typename T>
void CreatePath(bool isfinish, Node<T>* startNode, Node<T>* endNode, std::vector<Node<T>*> allFormerPaths);

template<typename T>
T* GetCell(sf::Vector2f pos, Grid<T>* grid);

template<typename T>
Node<T>* GetNode(sf::Vector2f pos, Grid<T>* grid);

template<typename T>
Node<T>* GetNodeInTab(int i, int j, int lenght_i, std::vector<Node<T>*>* allNodes);

template<typename A>
std::vector<A*>* EraseTab(std::vector<A*>* all);

template<typename A>
std::vector<std::vector<A*>>* EraseTab(std::vector<std::vector<A*>>* all);



















#include "Functions.inl"





