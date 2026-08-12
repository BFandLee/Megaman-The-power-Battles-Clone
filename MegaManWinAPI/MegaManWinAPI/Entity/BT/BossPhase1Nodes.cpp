#include "pch.h"
#include "BossPhase1Nodes.h"
#include "BossBlackboard.h"
#include "Boss.h"

// ---------------------------------------------------
// [Common] 공용 노드
// ---------------------------------------------------
NodeState BTAction_Common_JumpTo::Tick(Blackboard* bb)
{
    BossBlackboard* bossBB = static_cast<BossBlackboard*>(bb);
    if (!bossBB || !bossBB->OwnerBoss) return NodeState::Failure;
    Boss* boss = static_cast<Boss*>(bossBB->OwnerBoss);

    // TODO: boss->JumpTo()를 호출하고, 점프 중에는 Running, 완료 시 Success 반환
    return NodeState::Success;
}

// ---------------------------------------------------
// [Boss] 보스 전용 노드
// ---------------------------------------------------
NodeState BTAction_Boss_MagnetShield::Tick(Blackboard* bb)
{
    BossBlackboard* bossBB = static_cast<BossBlackboard*>(bb);
    if (!bossBB || !bossBB->OwnerBoss) return NodeState::Failure;
    Boss* boss = static_cast<Boss*>(bossBB->OwnerBoss);

    // TODO: boss->ActivateShield() 호출
    // 실드 활성화 시간 동안 Running 리턴하도록 구현하세요.
    return NodeState::Success;
}

// ---------------------------------------------------
// [Phase1] 페이즈 전용 노드
// ---------------------------------------------------
NodeState BTAction_Phase1_MagnetPull::Tick(Blackboard* bb)
{
    BossBlackboard* bossBB = static_cast<BossBlackboard*>(bb);
    if (!bossBB || !bossBB->OwnerBoss) return NodeState::Failure;
    Boss* boss = static_cast<Boss*>(bossBB->OwnerBoss);

    // TODO: 끌어당기기 시작 시 boss->SetInvincible(true) 호출
    // TODO: 진행 중일 때는 boss->PullPlayer() 호출 및 Running 리턴
    // TODO: 일정 시간 경과 후 boss->SetInvincible(false) 호출 및 Success 리턴
    return NodeState::Success;
}

NodeState BTAction_Phase1_MagnetMissile::Tick(Blackboard* bb)
{
    BossBlackboard* bossBB = static_cast<BossBlackboard*>(bb);
    if (!bossBB || !bossBB->OwnerBoss) return NodeState::Failure;
    Boss* boss = static_cast<Boss*>(bossBB->OwnerBoss);

    // Debug 로그
    OutputDebugString(L"[BossPhase1] 마그넷 미사일 발사 액션 도달!!!\n");
    // TODO: boss->ShootMagnetMissile() 호출
    // 애니메이션이 끝날 때까지 Running 리턴하도록 구현하세요.
    return NodeState::Success;
}
