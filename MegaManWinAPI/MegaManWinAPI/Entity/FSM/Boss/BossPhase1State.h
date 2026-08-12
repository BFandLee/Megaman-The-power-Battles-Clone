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
    // TODO: BTComponent를 멤버로 들고 페이즈 1용 트리를 실행하도록 할 수 있습니다.
    class BTNode* _btroot = nullptr;
};
