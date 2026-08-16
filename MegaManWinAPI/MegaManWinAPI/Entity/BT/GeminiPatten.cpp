#include "pch.h"
#include "GeminiPatten.h"
#include "Blackboard.h"
#include "BossClone.h" 
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Boss.h"
#include "TimeManager.h"

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
    float t = min(_spawnTimer / 0.5f, 1.0f);
    float currentX = lerp(_startPos.x, _targetPos.x, t);
    _spawnedClone->SetPos(Vector(currentX, _startPos.y));

    if (_spawnTimer >= 0.5f)
    {
        _spawnedClone->SetPos(_targetPos);
        _isSpawning = false;
        _spawnedClone = nullptr;
        return NodeState::Success;
    }

    return NodeState::Running;
}

NodeState BTAction_Gemini_CloneDeactivate::Tick(Blackboard* bb)
{
    if (bb->GetActor("BossClone", bb->BossClone) && bb->BossClone != nullptr)
    {
        SceneManager::GetInstance().GetScene()->DeleteActor(bb->BossClone);
        bb->SetActor("BossClone", nullptr);
    }
    
    return NodeState::Success;
    
}

NodeState BTAction_Gemini_LaserMissile::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 2: 레이저 미사일 발사
    // 1. SceneManager를 통해 `GeminiLaserProjectile` 액터를 Spawn합니다.
    // 2. 발사 방향을 45도 아래 등 튕기기 좋은 각도로 설정합니다.
    // (물리 및 반사 연산은 Projectile 자체 Update에서 수행하도록 위임합니다)
    return NodeState::Success;
}

NodeState BTAction_Gemini_SequentialJump::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 2: 연속 점프 이동
    // 점프를 두 번 연속으로 수행한 뒤 이동하는 시퀀스의 구체적 물리/상태 전이를 제어합니다.
    return NodeState::Success;
}
