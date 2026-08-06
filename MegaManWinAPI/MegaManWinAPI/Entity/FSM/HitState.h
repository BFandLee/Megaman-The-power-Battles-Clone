#pragma once
#include "State.h"

class HitState : public State
{
public:
	HitState(class FSMComponent* fsm);
	virtual ~HitState();

	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Exit() override;

private:
	float _hitTimer = 0.0f;
	float _maxhitTimer = 0.2f;
};
