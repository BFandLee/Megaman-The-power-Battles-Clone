#pragma once
#include "State.h"
class BossSpawn : public State
{
public:
    BossSpawn(class FSMComponent* fsm);
    virtual ~BossSpawn();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;

private:
    bool _isLanded = false;
    bool _isFirstFrame = true;
};

