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
    if (bb->OwnerBoss == nullptr || bb->BossAnimation == nullptr || bb->BossRigidBody == nullptr)
    {
        return NodeState::Failure;
    }


    if (!_isJumpStarted)
    {
        if (bb->OwnerBoss != nullptr)
        {
            bb->OwnerBoss->SetLookDirX(bb->DirXToPlayer);
        }

        bb->BossAnimation->Play(L"Jump");
        bb->BossRigidBody
          ->SetVelocity(Vector(bb->DirXToPlayer* 100.0f, -600.0f));

        _isJumpStarted = true;
        
        return NodeState::Running;
    }
    
    if (bb->BossRigidBody->IsGrounded())
    {
        bb->BossAnimation->Play(L"Idle");
        _isJumpStarted = false;
        return NodeState::Success;
    }

    return NodeState::Running;
    
}

NodeState BTAction_Gemini_BaseMissile::Tick(Blackboard* bb)
{

    if (bb->OwnerBoss == nullptr || bb->BossAnimation == nullptr || bb->BossTransform == nullptr)
    {
        return NodeState::Failure;
    }

    if (!_isAttackStarted)
    {
        bb->OwnerBoss->SetLookDirX(bb->DirXToPlayer);
        _isAttackFinished = false;

        bb->BossAnimation->SetEndEvent(L"Attack", [this]() {
            _isAttackFinished = true;
            });

        bb->BossAnimation->Play(L"Attack");

        // 본체 총알
        BossMissile* Missile = new BossMissile();
        Missile->Init();
        Missile->Fire(
            bb->BossTransform->GetPos(),
            Vector(bb->DirXToPlayer, 0.0f)
        );
        SceneManager::GetInstance().GetScene()->AddActor(Missile);

        // 클론 총알
        if (bb->BossClone != nullptr && bb->BossClone->GetActive())
        {
            Vector clonePos = bb->BossClone->GetComponent<TransformComponent>()->GetPos();
            float cloneDirX = 1.0f;
            if (bb->PlayerTransform != nullptr)
            {
                Vector playerPos = bb->PlayerTransform->GetPos();
                cloneDirX = (playerPos.x >= clonePos.x) ? 1.0f : -1.0f;
            }
            else
            {
                // 플레이어가 없을 땐 본체의 반대 방향(기본 대칭 방향) 유지
                cloneDirX = -bb->DirXToPlayer;
            }
            bb->BossClone->SetLookDirX(cloneDirX);
            BossMissile* CloneMissile = new BossMissile();
            CloneMissile->Init();
            CloneMissile->Fire(
                clonePos,
                Vector(cloneDirX, 0.0f)
            );
            SceneManager::GetInstance().GetScene()->AddActor(CloneMissile);
        }
        _isAttackStarted = true;
        return NodeState::Running;
    }
    else
    {
        if (bb->BossAnimation->GetCurrentClipName() != L"Attack")
        {
            _isAttackStarted = false;
            _isAttackFinished = false;
            return NodeState::Failure;

        }

        if (_isAttackFinished)
        {
            _isAttackStarted = false;
            _isAttackFinished = false;
            return NodeState::Success;
        }
    }
    
    return NodeState::Running;
}

NodeState BTAction_Gemini_Idle::Tick(Blackboard* bb)
{
    if (bb->OwnerBoss == nullptr || bb->BossAnimation == nullptr || bb->BossRigidBody == nullptr)
    {
        return NodeState::Failure;
    }
    bb->OwnerBoss->SetLookDirX(bb->DirXToPlayer);

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
    if (bb->OwnerBoss == nullptr || bb->BossAnimation == nullptr || bb->BossRigidBody == nullptr)
    {
        _isMoveStarted = false;
        return NodeState::Failure;
    }

    if (!_isMoveStarted)
    {
        _elpsedTime = 0.0f;
        _moveDirX = bb->DirXToPlayer;
        bb->OwnerBoss->SetLookDirX(_moveDirX);
        bb->BossAnimation->Play(L"Move");

        _isMoveStarted = true;
        return NodeState::Running;
    }

    _elpsedTime += TimeManager::GetInstance().GetDT();
    bb->BossRigidBody->SetVelocity(Vector(_moveDirX * 200.0f, bb->BossRigidBody->GetVelocity().y));

    if (_elpsedTime >= 1.5f)
    {
        _isMoveStarted = false;

        bb->BossRigidBody->SetVelocity(Vector(0.0f, 0.0f));

        return NodeState::Success;
    }
    return NodeState::Running;
}
