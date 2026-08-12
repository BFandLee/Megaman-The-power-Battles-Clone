#pragma once
#include "State.h"

class BossPhase2State : public State
{
public:
    BossPhase2State(class FSMComponent* fsm);
    virtual ~BossPhase2State();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;
};
