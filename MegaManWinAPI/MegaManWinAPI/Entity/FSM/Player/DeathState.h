#pragma once
#include "State.h"

class DeathState : public State
{
public:
	DeathState(class FSMComponent* fsm);
	virtual ~DeathState();

	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Exit() override;

private:
	float _deathTimer = 0.0f;
};
