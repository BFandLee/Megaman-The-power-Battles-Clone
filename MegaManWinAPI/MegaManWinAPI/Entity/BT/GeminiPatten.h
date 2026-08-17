#pragma once
#include "ActionNode.h"


// 분신 활성화 (분신 Actor Spawn)
class BTAction_Gemini_CloneActivate : public ActionNode
{
public:
    BTAction_Gemini_CloneActivate() { SetName("BTAction_Gemini_CloneActivate"); }
    virtual ~BTAction_Gemini_CloneActivate() = default;

    virtual NodeState Tick(class Blackboard* bb) override;

private:
    bool _isSpawning = false;
    float _spawnTimer = 0.0f;
    float _spawnDuration = 1.2f;
    Vector _startPos;
    Vector _targetPos;
    class BossClone* _spawnedClone = nullptr;
};

// 분신 해제 (분신 Actor Destroy)
class BTAction_Gemini_CloneDeactivate : public ActionNode
{
public:
    BTAction_Gemini_CloneDeactivate() { SetName("BTAction_Gemini_CloneDeactivate"); }
    virtual ~BTAction_Gemini_CloneDeactivate() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};

// 레이저 미사일 발사 (벽 튕김 투사체 생성)
class BTAction_Gemini_LaserMissile : public ActionNode
{
public:
    BTAction_Gemini_LaserMissile() { SetName("BTAction_Gemini_LaserMissile"); }
    virtual ~BTAction_Gemini_LaserMissile() = default;

    virtual NodeState Tick(class Blackboard* bb) override;

private:
    bool _isAttackStarted = false;
    bool _isAttackFinished = false;
};

// 연속 점프 이동 (기본 이동 패턴)
class BTAction_Gemini_SequentialJump : public ActionNode
{
public:
    BTAction_Gemini_SequentialJump() { SetName("BTAction_Gemini_SequentialJump"); }
    virtual ~BTAction_Gemini_SequentialJump() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};
