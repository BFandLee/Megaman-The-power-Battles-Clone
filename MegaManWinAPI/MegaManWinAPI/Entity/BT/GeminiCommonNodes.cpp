#include "pch.h"
#include "GeminiCommonNodes.h"
#include "Blackboard.h"
#include "Boss.h"
#include "Actor.h" 
#include "RigidBodyComponent.h" 
#include "TransformComponent.h" 
#include "AnimatorComponent.h" 
#include "BossMissile.h" 
#include "SceneManager.h" 
#include "Scene.h"
#include "TimeManager.h"

NodeState BTAction_Gemini_Jump::Tick(Blackboard* bb)
{
    Actor* player = bb->TargetPlayer;
    Actor* boss = bb->OwnerBoss;

    if (player == nullptr)
    {
        return NodeState::Failure;
    }

    if (!_isJumpStarted)
    {
        bb->OwnerBoss->GetComponent<AnimatorComponent>()->Play(L"Jump");
        bb->BossRigidBody
          ->SetVelocity(Vector(bb->DirXToPlayer* 100.0f, -600.0f));

        _isJumpStarted = true;
        
        return NodeState::Running;
    }
    
    if (bb->BossRigidBody->IsGrounded())
    {
        bb->OwnerBoss->GetComponent<AnimatorComponent>()->Play(L"Idle");
        _isJumpStarted = false;
        return NodeState::Success;
    }

    return NodeState::Running;
    
}

NodeState BTAction_Gemini_BaseMissile::Tick(Blackboard* bb)
{
    bb->OwnerBoss->GetComponent<AnimatorComponent>()->Play(L"Attack");
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

NodeState BTAction_Gemini_Idle::Tick(Blackboard* bb)
{
    if (bb->TargetPlayer == nullptr)
    {
        return NodeState::Failure;
    }

    if (!_isIdleStarted)
    {
        _elpsedTime = 0.0f;
        bb->OwnerBoss->GetComponent<AnimatorComponent>()->Play(L"Idle");
        bb->BossRigidBody
            ->SetVelocity(Vector(bb->DirXToPlayer * 0.0f, 0.0f));

        _isIdleStarted = true;
        return NodeState::Running;
    }

    _elpsedTime += TimeManager::GetInstance().GetDT();

    if (_elpsedTime >= 1.0f)
    {
        _isIdleStarted = false;
        return NodeState::Success;
    }
    return NodeState::Running;
}

NodeState BTAction_Gemini_Move::Tick(Blackboard* bb)
{
    if (bb->TargetPlayer == nullptr)
    {
        return NodeState::Failure;
    }

    if (!_isMoveStarted)
    {
        _elpsedTime = 0.0f;
        bb->OwnerBoss->GetComponent<AnimatorComponent>()->Play(L"Move");
        bb->BossRigidBody
          ->SetVelocity(Vector(bb->DirXToPlayer * 100.0f, 0.0f));

        _isMoveStarted = true;
        return NodeState::Running;
    }

    _elpsedTime += TimeManager::GetInstance().GetDT();

    if (_elpsedTime >= 1.5f)
    {
        _isMoveStarted = false;

        bb->BossRigidBody->SetVelocity(Vector(0.0f, 0.0f));

        return NodeState::Success;
    }
    return NodeState::Running;
}
