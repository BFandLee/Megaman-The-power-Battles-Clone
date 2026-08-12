#pragma once
#include "State.h"
class MoveState : public State
{
public:
    MoveState(FSMComponent* pOwner);
    virtual ~MoveState();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;
};



