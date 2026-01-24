#pragma once
#include <SFML/Graphics.hpp>

class Agent;

class Cell : public sf::Transformable
{
protected:
    sf::Vector2f mRowCol;

    bool mIsObstacle = false;
    bool mHasAgent = false;
    bool mHasCoin = false;

    Agent* mpAgent = nullptr;
public:
    Cell() { Start(); }

    void Start() { setPosition(0, 0); SetHasAgent(false); SetIsObstacle(false); SetHasCoin(false); SetAgent(nullptr); SetRowCol(-1, -1); }

    void SetAll(sf::Vector2f xy, bool obstacle, int row, int col) { setPosition(xy); SetIsObstacle(obstacle); SetRowCol(row, col); }
    
    void Update();
    void CheckStatus(int size);
    void Collide(int cellSize);

    const bool& HasCoin() const { return mHasCoin; }
    void SetHasCoin(bool state) { mHasCoin = state; }

    const bool& HasAgent() const { return mHasAgent; }
    void SetHasAgent(bool state) { mHasAgent = state; }

    Agent* GetAgent() { return mpAgent; }
    void SetAgent(Agent* a) { mpAgent = a; }

    const bool& IsObstacle() const { return mIsObstacle; }
    void SetIsObstacle(bool obstacle) { mIsObstacle = obstacle; }

    const sf::Vector2f& GetRowCol() const { return mRowCol; }
    void SetRowCol(int row, int col) { mRowCol.x = row; mRowCol.y = col; }
};


