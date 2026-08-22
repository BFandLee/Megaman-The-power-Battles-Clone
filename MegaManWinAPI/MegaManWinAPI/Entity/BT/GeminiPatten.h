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
    enum class SpawnStep
    {
        None,
        JumpToCenter, // 1단계: 중앙 공중으로 상승
        ApexPause,    // 2단계: 정점 체공 및 분신 스폰
        SplitLanding  // 3단계: 양옆으로 갈라져 하강 착지
    };

    SpawnStep _step = SpawnStep::None;
    float _stepTimer = 0.0f;

    Vector _startPos;
    Vector _apexPos;       // 중앙 상공 정점 좌표
    Vector _bossLandPos;   // 본체 착지 목표 좌표 (좌측)
    Vector _cloneLandPos;  // 분신 착지 목표 좌표 (우측)

    class BossClone* _spawnedClone = nullptr;
};

// 분신 해제 (분신 Actor Destroy)
class BTAction_Gemini_CloneDeactivate : public ActionNode
{
public:
    BTAction_Gemini_CloneDeactivate() { SetName("BTAction_Gemini_CloneDeactivate"); }
    virtual ~BTAction_Gemini_CloneDeactivate() = default;

    virtual NodeState Tick(class Blackboard* bb) override;

private:
    enum class DeactivateStep
    {
        None,
        JumpToCenter, // 1단계: 본체와 분신이 중앙 공중으로 상승 및 합체
        CenterLanding // 2단계: 본체 중앙 하강 착지
    };

    DeactivateStep _step = DeactivateStep::None;
    float _stepTimer = 0.0f;

    Vector _bossStartPos;
    Vector _cloneStartPos;
    Vector _apexPos;       // 중앙 상공 정점 좌표
    Vector _landPos;       // 본체 착지 목표 좌표 (화면 중앙 바닥)
    class BossClone* _targetClone = nullptr;
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
