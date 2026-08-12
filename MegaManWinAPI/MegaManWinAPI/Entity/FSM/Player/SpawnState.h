#pragma once
#include "State.h"

class SpawnState : public State
{
public:
    SpawnState(class FSMComponent* fsm) : State(fsm) {}
    virtual ~SpawnState() = default;

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;

private:
    bool _isLanded = false;
    bool _isFirstFrame = true;
};