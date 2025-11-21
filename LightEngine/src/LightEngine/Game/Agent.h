#pragma once
#include "../Entity.h"
#include "PathFinding.h"
#include <SFML/Graphics/CircleShape.hpp>

class MainScene;
class Grid;
class MainScene;

class Agent : public Entity
{
protected:
	PathFinding<Cell> mPath;
	int mIndexPath;
	bool mRoam;

	Node<Cell>* mStartNode;
	Node<Cell>* mEndNode;

	sf::Color mClassicColor;
	sf::Color mRoamColor;

	MainScene* mCurrentScene;

	bool mNeedToWaitForReset = false;

	float mWaitTime = 1.f;
	float mWaitProgress = 0.f;

	bool mIsGhost = false; 
public:
	virtual void OnInitialize();

	virtual void OnUpdate();

	virtual void OnPathFinish();

	virtual void OnDestroy();

	virtual void UpdatePath();

	void GoToCell(Cell* cellEnd, Grid* grid);
	void GoToCell(sf::Vector2f pos, Grid* grid);

	void GoToNode(Node<Cell>* nodeEnd, Grid* grid);
	void GoToNode(Node<Cell>* startNode, Node<Cell>* endNode, Grid* grid);

	void ResetPath(Node<Cell>* endNode, Grid* grid);

	int GetCurrentIndexOnPath(Node<Cell>* currentNode, std::vector<Node<Cell>*>* newAllPaths);

	void SetPathfinding(PathFinding<Cell> path) { mPath = path; }
	PathFinding<Cell>* GetPathfinding() { return &mPath; }

	void SetStartNode(Node<Cell>* startNode) { mStartNode = startNode; }
	Node<Cell>* GetStartNode() { return mStartNode; }

	void SetEndNode(Node<Cell>* endNode) { mEndNode = endNode; }
	Node<Cell>* GetEndNode() { return mEndNode; }

	void SetRoam(bool roam);
	const bool& GetRoam() const { return mRoam; }  

	void SetGhost(bool state) { mIsGhost = state; } 
	const bool& IsGhost() const { return mIsGhost;  }  

	void SetClassicColor(sf::Color color) { mClassicColor = color; } 
	const sf::Color& GetClassicColor() const { return mClassicColor; } 

	void SetRoamColor(sf::Color color) { mRoamColor = color; }
	const sf::Color& GetRoamColor() const { return mRoamColor; }

	void SetCurrentScene(MainScene* currentScene) { mCurrentScene = currentScene; }
	MainScene* GetCurrentScene() { return mCurrentScene; }
};
