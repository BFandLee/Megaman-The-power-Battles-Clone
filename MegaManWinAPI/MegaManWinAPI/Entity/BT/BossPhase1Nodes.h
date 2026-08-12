#pragma once
#include "ActionNode.h" // 기존 ConditionNode가 있다면 상속 구조를 맞게 변경하세요.

// ---------------------------------------------------
// [Common] 공용 노드: 모든 몬스터가 사용할 수 있는 노드
// ---------------------------------------------------



class BTAction_Common_JumpTo : public ActionNode
{
public:
    BTAction_Common_JumpTo() { SetName("BTAction_Common_JumpTo"); }
    virtual ~BTAction_Common_JumpTo() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};

// ---------------------------------------------------
// [Boss] 보스 전용 노드: 특정 보스의 여러 페이즈에서 재사용
// ---------------------------------------------------

class BTAction_Boss_MagnetShield : public ActionNode
{
public:
    BTAction_Boss_MagnetShield() { SetName("BTAction_Boss_MagnetShield"); }
    virtual ~BTAction_Boss_MagnetShield() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};

// ---------------------------------------------------
// [Phase1] 페이즈 전용 노드: Phase 1 특수 패턴
// ---------------------------------------------------

class BTAction_Phase1_MagnetPull : public ActionNode
{
public:
    BTAction_Phase1_MagnetPull() { SetName("BTAction_Phase1_MagnetPull"); }
    virtual ~BTAction_Phase1_MagnetPull() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};

class BTAction_Phase1_MagnetMissile : public ActionNode
{
public:
    BTAction_Phase1_MagnetMissile() { SetName("BTAction_Phase1_MagnetMissile"); }
    virtual ~BTAction_Phase1_MagnetMissile() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};
