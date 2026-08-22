#pragma once
#include "State.h"

class SlideState : public State
{
private:
    float _slideVelocity;
    float _friction;
    float _originalHeight;
    float _originalOffsetY;



public:
    SlideState(class FSMComponent* pOwner);
    virtual ~SlideState();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;
};
