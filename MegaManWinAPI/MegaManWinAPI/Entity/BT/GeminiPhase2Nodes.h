#pragma once
#include "ActionNode.h"

// ===================================================
// [Gemini Man] Phase 2 Action Nodes
// ===================================================

// 분신 해제 (분신 Actor Destroy)
class BTAction_Gemini_CloneDeactivate : public ActionNode
{
public:
    BTAction_Gemini_CloneDeactivate() { SetName("BTAction_Gemini_CloneDeactivate"); }
    virtual ~BTAction_Gemini_CloneDeactivate() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};

// 분신 소환 (분신 Actor 재Spawn)
class BTAction_Gemini_CloneSummon : public ActionNode
{
public:
    BTAction_Gemini_CloneSummon() { SetName("BTAction_Gemini_CloneSummon"); }
    virtual ~BTAction_Gemini_CloneSummon() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};

// 레이저 미사일 발사 (벽 튕김 투사체 생성)
class BTAction_Gemini_LaserMissile : public ActionNode
{
public:
    BTAction_Gemini_LaserMissile() { SetName("BTAction_Gemini_LaserMissile"); }
    virtual ~BTAction_Gemini_LaserMissile() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};

// 연속 점프 이동 (기본 이동 패턴)
class BTAction_Gemini_SequentialJump : public ActionNode
{
public:
    BTAction_Gemini_SequentialJump() { SetName("BTAction_Gemini_SequentialJump"); }
    virtual ~BTAction_Gemini_SequentialJump() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};
