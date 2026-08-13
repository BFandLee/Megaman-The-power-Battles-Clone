#pragma once
#include "ActionNode.h"

// 점프 이동
class BTAction_Gemini_JumpMove : public ActionNode
{
public:
    BTAction_Gemini_JumpMove() { SetName("BTAction_Gemini_JumpMove"); }
    virtual ~BTAction_Gemini_JumpMove() = default;

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
};

namespace
{
    float DirX = 0.0f;
    Actor* Player;
    Actor* Boss;
}

