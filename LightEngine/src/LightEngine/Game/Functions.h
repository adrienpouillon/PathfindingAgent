#pragma once

#include "../Utils.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

#define RACINE std::sqrt(2)

class Grid;
class Agent;
class Entity;

template<typename T>
class Node;

template<typename A>
void AddVector2OnVector1(std::vector<A>* vector1, const std::vector<A>& vector2);

template<typename A>
void AddReveseVector2OnVector1(std::vector<A>* vector1, const std::vector<A>& vector2);

template<typename T>
void FindPath(Node<T>* startNode, Node<T>* endNode, Grid* grid, std::vector<Node<T>*>* allFormerPaths, Agent* pOwner);

template<typename T>
void CreatePath(bool isfinish, Node<T>* startNode, Node<T>* endNode, std::vector<Node<T>*>* allFormerPaths);

template<typename T>
void ResetNodes(Grid* grid);

template<typename A>
std::vector<A*>* EraseTab(std::vector<A*>* all);

template<typename A>
std::vector<std::vector<A*>>* EraseTab(std::vector<std::vector<A*>>* all);

template<typename T>
T* GetCell(sf::Vector2f pos, Grid* grid);

template<typename T>
Node<T>* GetNode(sf::Vector2f pos, Grid* grid);

template<typename T>
Node<T>* GetNodeInTab(int row, int col, int length_col, std::vector<Node<T>*>* allNodes);

Entity* GetNearestEntity(sf::Vector2f pos);

template<typename T>
T* GetNearestCell(sf::Vector2f pos, std::vector<std::vector<T*>> allCells);

template<typename T>
T* GetEntityNearest(sf::Vector2f pos, float minDis);

#include "Functions.inl"





