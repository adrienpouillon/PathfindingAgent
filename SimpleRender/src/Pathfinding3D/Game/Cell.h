#pragma once
#include "../LightEngine/Entity.h"

#define CELL_HEIGHT_UP 10.f
#define CELL_HEIGHT_COIN 25.f

class Agent;

class Cell : public Entity
{
protected:
    int mConfigHeight = 1; // 0 = Hole | 1 = Classic Floor | > 1 = Wall + n
    gce::Vector2i32 mRowCol;

    bool mHasAgent;
    bool mHasCoin;

    Agent* mpAgent;
    Entity* mEntityCoin;
public:
    Cell() { Start(); }

    void Start();

    void SetAll(gce::Vector3f32 pos, int height, int row, int col) { SetPosition(pos.x, pos.y, pos.z); SetConfigHeight(height); SetRowCol(row, col); }
    
    void OnUpdate() override;
    void CheckStatus(int size);

    void ChangeHeight(int newHeight);
    void PrintCoin();

    Entity* CreateCoin();

    const bool& HasAgent() const { return mHasAgent; }
    void SetHasAgent(bool state) { mHasAgent = state; }

    const bool& HasCoin() const { return mHasCoin; }
    void SetHasCoin(bool state) { mHasCoin = state; }

    Agent* GetAgent() { return mpAgent; }
    void SetAgent(Agent* a) { mpAgent = a; }

    Entity* GetCoin() { return mEntityCoin; }
    void SetCoin(Entity* coin) { mEntityCoin = coin; }

    const int& GetConfigHeight() const { return mConfigHeight; }
    void SetConfigHeight(int height) { mConfigHeight = height; }

    const int& GetRealHeight() const { return CELL_HEIGHT_UP * mConfigHeight; }

    const bool& IsObstacle() const { return GetConfigHeight() <= 0; }

    const gce::Vector2i32& GetRowCol() const { return mRowCol; }
    void SetRowCol(int row, int col) { mRowCol.x = row; mRowCol.y = col; }

    ~Cell() = default;
};


