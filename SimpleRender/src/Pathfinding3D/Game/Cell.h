#pragma once
#include "../LightEngine/Entity.h"

constexpr float CellHeight = 10.f;
class Agent;

class Cell : public Entity
{
protected:
    int mConfigHeight = 1; // 0 = Hole | 1 = Classic Floor | > 1 = Wall + n

    gce::Vector2i32 mRowCol;

    bool mHasAgent = false;

    Agent* mpAgent = nullptr;
public:
    Cell() { Start(); }

    void Start() { SetConfigHeight(1);  SetRowCol(-1, -1); }

    void SetAll(gce::Vector3f32 pos, int height, int row, int col) { SetPosition(pos.x, pos.y, pos.z); SetConfigHeight(height); SetRowCol(row, col); }
    
    void OnUpdate() override;
    void CheckStatus(int size);

    void ChangeHeight(int newHeight);

    const bool& HasAgent() const { return mHasAgent; }
    void SetHasAgent(bool state) { mHasAgent = state; }

    Agent* GetAgent() { return mpAgent; }
    void SetAgent(Agent* a) { mpAgent = a; }

    const int& GetConfigHeight() const { return mConfigHeight; }
    void SetConfigHeight(int height) { mConfigHeight = height; }

    const int& GetRealHeight() const { return CellHeight * mConfigHeight; }

    const bool& IsObstacle() const { return GetConfigHeight() <= 0; }

    const gce::Vector2i32& GetRowCol() const { return mRowCol; }
    void SetRowCol(int row, int col) { mRowCol.x = row; mRowCol.y = col; }

    ~Cell() = default;
};


