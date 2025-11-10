#pragma once
#include "Vector2.h"

class Cell
{
protected:
    char mChara;
    Vector2 mXY;
    bool mObstacle;
public:
    Cell() { Start(); }

    void Start() { SetChara(' '); SetXY(0, 0); SetObstacle(false); }

    void SetAll(char chara, Vector2 xy, bool obstacle) { SetChara(chara); SetXY(xy); SetObstacle(obstacle); }

    void SetChara(char chara) { mChara = chara; }
    char GetChara() { return mChara; }

    void SetXY(Vector2 xy) { mXY = xy; }
    Vector2 GetXY() { return mXY; }

    void SetObstacle(bool obstacle) { mObstacle = obstacle; }
    bool GetObstacle() { return mObstacle; }

    ~Cell() {}
};

