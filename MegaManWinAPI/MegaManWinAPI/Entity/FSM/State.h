#pragma once

class FSMComponent;

class State
{
protected:
    FSMComponent* m_pOwnerFSM = nullptr;
    class AnimatorComponent* _pAnimator = nullptr;
public:
    State(FSMComponent* pOwner) : m_pOwnerFSM(pOwner) {}
    virtual ~State() {}

    // 하위 상태 클래스들이 반드시 구현해야 할 가상 함수들
    virtual void Enter() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Exit() = 0;
};
