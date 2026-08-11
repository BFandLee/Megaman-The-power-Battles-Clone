#pragma once
#include "State.h"
class JumpState : public State
{
public:
    JumpState(FSMComponent* pOwner) :State(pOwner) {}
    virtual ~JumpState();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;

private:
    class RigidBodyComponent* _rigidbody;
    class MovementComponent* _movement;
};

