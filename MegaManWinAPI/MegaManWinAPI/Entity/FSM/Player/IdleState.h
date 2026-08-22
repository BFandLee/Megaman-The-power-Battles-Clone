#pragma once
#include "State.h"

class IdleState : public State
{
public:
    IdleState(FSMComponent* pOwner);
    virtual ~IdleState();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;
};
