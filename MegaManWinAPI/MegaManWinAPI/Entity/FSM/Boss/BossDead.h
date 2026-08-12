#pragma once
#include "State.h"
class BossDead : public State
{
public:
    BossDead(class FSMComponent* fsm);
    virtual ~BossDead();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;
};