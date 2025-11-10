#pragma once
#include <SFML/Graphics.hpp>

class Cell : public sf::Transformable
{
protected:
    bool mObstacle = false;
    bool mAgent = false;
public:
    Cell() { Start(); }

    void Start() { setPosition(0,0) ; SetObstacle(false); }

    void SetAll(sf::Vector2f xy, bool obstacle) { setPosition(xy); SetObstacle(obstacle); }

    void SetObstacle(bool obstacle) { mObstacle = obstacle; }
    const bool& GetObstacle() const { return mObstacle; }

    ~Cell() = default;
};


