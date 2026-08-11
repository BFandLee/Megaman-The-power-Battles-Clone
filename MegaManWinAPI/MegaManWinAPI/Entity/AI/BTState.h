// BTState.h
#pragma once
#include "State.h"

// 전방 선언
class BTNode;

class BTState : public State
{
    using Super = State;
public:
    BTState(class FSMComponent* pOwner) : State(pOwner) {}
    virtual ~BTState() {}

    virtual void Update(float deltaTime) override;
    virtual void Enter() override;
    virtual void Exit() override;

    // ImGui 시각화를 위한 함수 (선택 사항)
    virtual void RenderUI();

    void SetRootNode(BTNode* node) { _rootNode = node; }

protected:
    BTNode* _rootNode = nullptr;
};