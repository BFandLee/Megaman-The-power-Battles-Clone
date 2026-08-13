#include "pch.h"
#include "GeminiCommonNodes.h"
#include "Blackboard.h"
#include "Boss.h"
#include "Actor.h" 
#include "RigidBodyComponent.h" 
#include "TransformComponent.h" 
#include "BossMissile.h" 
#include "SceneManager.h" 
#include "Scene.h" 

NodeState BTAction_Gemini_JumpMove::Tick(Blackboard* bb)
{
    Actor* player = bb->TargetPlayer;
    Actor* boss = bb->OwnerBoss;

    if (player == nullptr)
    {
        return NodeState::Failure;
    }

    if (!_isJumpStarted)
    {
        bb->BossRigidBody
          ->SetVelocity(Vector(bb->DirXToPlayer* 100.0f, -600.0f));

        _isJumpStarted = true;
        return NodeState::Running;
    }
    
    if (bb->BossRigidBody->IsGrounded() == true)
    {
        _isJumpStarted = false;
        return NodeState::Success;
    }

    return NodeState::Running;
    
}

NodeState BTAction_Gemini_BaseMissile::Tick(Blackboard* bb)
{
    // [코칭 가이드] 공통: 기본 미사일 발사
    // 1. 기본 미사일 투사체(Projectile)를 씬에 Spawn합니다.
    // 2. 발사 방향은 보스가 바라보는 방향 또는 타겟을 향하게 설정합니다.
    
    // (구현) ...
    BossMissile* Missile = new BossMissile();
    Missile->Fire(
        bb->BossTransform->GetPos(), 
        Vector(bb->DirXToPlayer, 0.0f),
        50.0f,
        5.0f
        );
    SceneManager::GetInstance().GetScene()->AddActor(Missile);
    return NodeState::Success; // 발사 즉시 완료라고 가정
}
