#pragma once
#include "State.h"

class BossPhase1State : public State
{
public:
    BossPhase1State(class FSMComponent* fsm);
    virtual ~BossPhase1State();

    virtual void Enter() override;
    virtual void Update(float deltaTime) override;
    virtual void Exit() override;

private:
    class BTNode* _rootNode = nullptr;
    class Boss* _boss = nullptr;
};
