#pragma once
#include <vector>
#include <string>
#include "Cell.h"
#include "Node.h"
#include "GridConfigs.h"

struct AgentsData
{
	sf::Vector2f pos = { 0, 0 };
	float speed = -1;
	float radius = -1;
	sf::Color classicColor = sf::Color::White;
	sf::Color roamColor = sf::Color::Black;

	AgentsData(sf::Vector2f _pos, float _speed, float _radius, sf::Color _classicColor, sf::Color _roamColor)
	{
		pos = _pos;
		speed = _speed;
		radius = _radius;
		classicColor = _classicColor;
		roamColor = _roamColor;
	}
};

class MainScene;

class Grid
{
protected:
	int mCellSize;
	std::vector<Node<Cell>*> mAllNodes;
	std::vector<std::vector<Cell*>> mAllCells;

	MainScene* pCurrentScene = nullptr;

	std::vector<AgentsData> mGuardsDataFromTxt;
	std::vector<AgentsData> mCivilDataFromTxt;
	std::vector<AgentsData> mAssassinDataFromTxt;
public:
	Grid() { Start(); }

	void Start();
	void InitTab(std::string fileName);
	void InitTab(int _rows, int _cols, std::string strGrid = "");
	void InitNodeNeighbor();

	void AddAgentsFromTxt();

	void CleanGrid();

	void SaveGrid(std::string fileName);
	std::string GetStringFromTxt(std::string fileName);

	void Update();
	void UpdateCellsStatut();

	void DrawGrid();
	void DrawColorCell();
	void DrawLineGrid();

	sf::Vector2f GetPositionToView(int rows, int cols, float coef, int size);

	void SetParameters(int cellSize, bool usingTxt = false);
	const int& GetCellSize() const { return mCellSize; }

	std::vector<std::vector<Cell*>>& GetAllCells() { return mAllCells; }

	std::vector<Node<Cell>*>& GetAllNodes() { return mAllNodes; }

protected:
	void CreateTab(int _rows, int _cols, std::string strGrid);
	void InitNodeNeighbor(int rows, int cols);

};