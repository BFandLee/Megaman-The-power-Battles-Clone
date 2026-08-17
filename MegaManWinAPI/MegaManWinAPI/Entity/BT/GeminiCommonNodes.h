#pragma once
#include "ActionNode.h"

// 점프
class BTAction_Gemini_Jump : public ActionNode
{
public:
    BTAction_Gemini_Jump() { SetName("BTAction_Gemini_JumpMove"); }
    virtual ~BTAction_Gemini_Jump() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
private:
    bool _isJumpStarted = false;
   
};

// 기본 미사일 발사
class BTAction_Gemini_BaseMissile : public ActionNode
{
public:
    BTAction_Gemini_BaseMissile() { SetName("BTAction_Gemini_BaseMissile"); }
    virtual ~BTAction_Gemini_BaseMissile() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
private:
    bool _isAttackStarted = false;
    bool _isAttackFinished = false;
};

// 대기 상태
class BTAction_Gemini_Idle: public ActionNode
{
public:
    BTAction_Gemini_Idle() { SetName("BTAction_Gemini_Idle"); }
    virtual ~BTAction_Gemini_Idle() = default;

    virtual NodeState Tick(class Blackboard* bb) override;

private:
    bool _isIdleStarted = false;
    float _elpsedTime = 0.0f;
};

// 이동
class BTAction_Gemini_Move : public ActionNode
{
public:
    BTAction_Gemini_Move() { SetName("BTAction_Gemini_JumpMove"); }
    virtual ~BTAction_Gemini_Move() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
private:
    bool _isMoveStarted = false;
    float _elpsedTime = 0.0f;
    float _moveDirX = 1.0f;
};