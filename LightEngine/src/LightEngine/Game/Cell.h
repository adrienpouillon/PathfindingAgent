#pragma once
#include <SFML/Graphics.hpp>

class Cell : public sf::Transformable
{
protected:
    int mSize;
    bool mObstacle = false;
    bool mAgent = false;
public:
    Cell() { Start(); }

    void Start() { setPosition(0, 0); SetObstacle(false); mSize = 1; }

    void SetAll(sf::Vector2f xy, bool obstacle) { setPosition(xy); SetObstacle(obstacle); }

    void Update();
    void CheckStatus();

    const int& GetSize() { return mSize; }
    void SetSize(int size) { mSize = size; }

    const bool& GetAgent() { return mAgent; }
    void SetAgent(bool state) { mAgent = state; }

    void SetObstacle(bool obstacle) { mObstacle = obstacle; }
    const bool& GetObstacle() const { return mObstacle; }

    ~Cell() = default;
};


