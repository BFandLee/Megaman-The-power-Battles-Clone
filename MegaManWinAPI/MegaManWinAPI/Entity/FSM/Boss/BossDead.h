#pragma once
#include "State.h"

class Boss;

class BossDead : public State
{
public:
    BossDead(class FSMComponent* fsm);
    virtual ~BossDead();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;

private:
    Boss* _boss = nullptr;
    float _deadTimer = 0.0f;
    float _maxDeadTime = 1.5f;
    bool _isExploded = false;
};