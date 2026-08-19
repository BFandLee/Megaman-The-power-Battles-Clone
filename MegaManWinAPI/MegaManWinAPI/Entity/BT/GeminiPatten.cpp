#include "pch.h"
#include "GeminiPatten.h"
#include "Blackboard.h"
#include "BossClone.h" 
#include "SceneManager.h"
#include "TimeManager.h"
#include "TransformComponent.h"
#include "AnimatorComponent.h"
#include "Scene.h"
#include "Boss.h"
#include "BossMissile.h"

NodeState BTAction_Gemini_CloneActivate::Tick(Blackboard* bb)
{
    if (!_isSpawning)
    {
        if (SceneManager::GetInstance().GetScene()->FindActorByType(ActorType::BossClone))
        {
            return NodeState::Success;
        }

        Vector bossPos = bb->BossTransform->GetPos();
        float targetX = (2.0f * (GWinSizeX / 2.0f)) - bossPos.x;
        int cloneCount = 0;
        for (Actor* actor : SceneManager::GetInstance().GetScene()->GetActors())
        {
            if (actor->GetActorType() == ActorType::BossClone)
            {
                cloneCount++;

                if (cloneCount >= 1)
                {
                    return NodeState::Success;
                }
            }
        }

        BossClone* clone = new BossClone();
        Vector bossScale = bb->BossTransform->GetScale();
        clone->Init();
        clone->SetPos(bossPos);
        clone->SetOwner(static_cast<Boss*>(bb->OwnerBoss));
        clone->SetScale(Vector(abs(bossScale.x), abs(bossScale.y)));
        SceneManager::GetInstance().GetScene()->AddActor(clone);
        bb->SetActor("BossClone", clone);

        _startPos = bossPos;
        _targetPos = Vector(targetX, bossPos.y);
        _spawnTimer = 0.0f;
        _spawnedClone = clone;
        _isSpawning = true;
        return NodeState::Running;
    }
    
    _spawnTimer += TimeManager::GetInstance().GetDT();
     float t = min(_spawnTimer / _spawnDuration, 1.0f);

     // Ease-Out Cubic(EaseOut 세제곱) 보간
     float invT = 1.0f - t;
     float easedT = 1.0f - (invT * invT * invT);

    float currentX = lerp(_startPos.x, _targetPos.x, easedT);
    _spawnedClone->SetPos(Vector(currentX, _startPos.y));

    if (_spawnTimer >= _spawnDuration)
    {
        _spawnedClone->SetPos(_targetPos);
        _isSpawning = false;
        _spawnedClone = nullptr;

        bb->SetFloat("CloneSpawnTime", TimeManager::GetInstance().GetGlobalTime());
        return NodeState::Success;
    }

    return NodeState::Running;
}

NodeState BTAction_Gemini_CloneDeactivate::Tick(Blackboard* bb)
{
    if (bb->BossClone != nullptr)
    {
        SceneManager::GetInstance().GetScene()->DeleteActor(bb->BossClone);
        bb->SetActor("BossClone", nullptr);
    }
    
    return NodeState::Success;
    
}

NodeState BTAction_Gemini_LaserMissile::Tick(Blackboard* bb)
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

        Vector fireDir = Vector(bb->DirXToPlayer, -1.0f);
        fireDir.Normalize();

        BossMissile* missile = new BossMissile();
        if (missile != nullptr)
        {
            missile->Init();
            missile->Fire(bb->BossTransform->GetPos(), fireDir, MissileType::Razer);
            SceneManager::GetInstance().GetScene()->AddActor(missile);
            _isAttackStarted = true;
            return NodeState::Running;
        }
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

NodeState BTAction_Gemini_SequentialJump::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 2: 연속 점프 이동
    // 점프를 두 번 연속으로 수행한 뒤 이동하는 시퀀스의 구체적 물리/상태 전이를 제어합니다.
    return NodeState::Success;
}
