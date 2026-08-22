#include "pch.h"
#include "GeminiPatten.h"
#include "Blackboard.h"
#include "BossClone.h" 
#include "SceneManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "TransformComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "Scene.h"
#include "Boss.h"
#include "BossMissile.h"

NodeState BTAction_Gemini_CloneActivate::Tick(Blackboard* bb)
{
    if (bb == nullptr || bb->OwnerBoss == nullptr || bb->BossTransform == nullptr)
        return NodeState::Failure;
    Boss* boss = static_cast<Boss*>(bb->OwnerBoss);
    float dt = TimeManager::GetInstance().GetDT();

    if (bb->BossRigidBody)
    {
        bb->BossRigidBody->SetVelocity(Vector(0.0f, 0.0f));
    }

    if (_step == SpawnStep::None)
    {
        // 이미 분신이 소환되어 있다면 성공 처리
        if (SceneManager::GetInstance().GetScene()->FindActorByType(ActorType::BossClone))
            return NodeState::Success;

        boss->SetSuperArmor(true);
        _startPos = bb->BossTransform->GetPos();
        float groundY = _startPos.y; // 바닥 높이
        
        // 1) 중앙 상공 정점 좌표 (화면 중앙 X, 바닥보다 260px 높은 위치)
        _apexPos = Vector(GWinSizeX * 0.5f, groundY - 260.0f);
        
        // 2) 좌/우 착지 목표 좌표 (본체는 좌측 25%, 분신은 우측 75%)
        _bossLandPos = Vector(GWinSizeX * 0.25f, groundY);
        _cloneLandPos = Vector(GWinSizeX * 0.75f, groundY);
        if (bb->BossAnimation)
            bb->BossAnimation->Play(L"Jump");
        _stepTimer = 0.0f;
        _step = SpawnStep::JumpToCenter;
        return NodeState::Running;
    }
    _stepTimer += dt;
    
    if (_step == SpawnStep::JumpToCenter)
    {
        const float duration = 0.5f;
        float t = min(_stepTimer / duration, 1.0f);
        
        // Ease-Out으로 정점으로 부드럽게 솟아오름
        float easedT = 1.0f - (1.0f - t) * (1.0f - t);
        Vector currentPos = Vector(lerp(_startPos.x, _apexPos.x, easedT),
                                   lerp(_startPos.y, _apexPos.y, easedT));
        boss->SetPos(currentPos);
        if (_stepTimer >= duration)
        {
            boss->SetPos(_apexPos);
               // ③ 정점 위치에 분신(Clone) 생성
            BossClone* clone = new BossClone();
            Vector bossScale = bb->BossTransform->GetScale();
            clone->Init();
            clone->SetPos(_apexPos);
            clone->SetOwner(boss);
            clone->SetScale(bossScale);

            // 본체의 애니메이션 클립, 현재 프레임 번호, 누적 시간 완벽 복제
            AnimatorComponent* bossAnim = bb->BossAnimation;
            AnimatorComponent* cloneAnim = clone->GetComponent<AnimatorComponent>();
            if (bossAnim && cloneAnim)
            {
                cloneAnim->SyncFrom(bossAnim);
            }

            SceneManager::GetInstance().GetScene()->AddActor(clone);
            bb->SetActor("BossClone", clone);
            _spawnedClone = clone;
            _stepTimer = 0.0f;
            _step = SpawnStep::ApexPause;
        }
        return NodeState::Running;
    }

    if (_step == SpawnStep::ApexPause)
    {
        boss->SetPos(_apexPos);
        if (_spawnedClone)
            _spawnedClone->SetPos(_apexPos);

        const float duration = 0.25f;
        if (_stepTimer >= duration)
        {
            _stepTimer = 0.0f;
            _step = SpawnStep::SplitLanding;
        }
        return NodeState::Running;
    }

    if (_step == SpawnStep::SplitLanding)
    {
        const float duration = 0.5f;
        float t = min(_stepTimer / duration, 1.0f);
        
        // Ease-In으로 낙하 가속 느낌 연출
        float easedT = t * t;
        // 본체: 좌측 바닥으로 이동
        Vector bossPos = Vector(lerp(_apexPos.x, _bossLandPos.x, easedT),
                                lerp(_apexPos.y, _bossLandPos.y, easedT));
        boss->SetPos(bossPos);
        // 분신: 우측 바닥으로 이동
        if (_spawnedClone)
        {
            Vector clonePos = Vector(lerp(_apexPos.x, _cloneLandPos.x, easedT),
                                     lerp(_apexPos.y, _cloneLandPos.y, easedT));
            _spawnedClone->SetPos(clonePos);
        }
        // 착지 완료
        if (_stepTimer >= duration)
        {
            boss->SetPos(_bossLandPos);
            if (_spawnedClone) _spawnedClone->SetPos(_cloneLandPos);
            if (bb->BossRigidBody)
            {
                bb->BossRigidBody->SetVelocity(Vector(0.0f, 0.0f));
                bb->BossRigidBody->SetGrounded(true);
            }
            if (_spawnedClone)
            {
                RigidBodyComponent* cloneRigid = _spawnedClone->GetComponent<RigidBodyComponent>();
                if (cloneRigid)
                {
                    cloneRigid->SetVelocity(Vector(0.0f, 0.0f));
                    cloneRigid->SetGrounded(true);
                }
                _spawnedClone->SetLookDirX(bb->DirXToPlayer);
            }
            if (bb->BossAnimation) bb->BossAnimation->Play(L"Idle");
            boss->SetLookDirX(bb->DirXToPlayer);
            boss->SetSuperArmor(false);
            _step = SpawnStep::None;
            _spawnedClone = nullptr;
            bb->SetFloat("CloneSpawnTime", TimeManager::GetInstance().GetGlobalTime());
            return NodeState::Success;
        }
        return NodeState::Running;
    }
    return NodeState::Running;
}

NodeState BTAction_Gemini_CloneDeactivate::Tick(Blackboard* bb)
{
    if (bb == nullptr || bb->OwnerBoss == nullptr || bb->BossTransform == nullptr)
        return NodeState::Failure;

    Boss* boss = static_cast<Boss*>(bb->OwnerBoss);
    float dt = TimeManager::GetInstance().GetDT();

    // 연출이 진행되는 동안 RigidBody 중력 가속도 누적 방지
    if (bb->BossRigidBody)
    {
        bb->BossRigidBody->SetVelocity(Vector(0.0f, 0.0f));
    }

    if (_step == DeactivateStep::None)
    {
        Actor* cloneActor = nullptr;
        bb->GetActor("BossClone", cloneActor);
        if (cloneActor == nullptr)
            cloneActor = SceneManager::GetInstance().GetScene()->FindActorByType(ActorType::BossClone);

        if (cloneActor == nullptr)
        {
            boss->SetSuperArmor(false);
            boss->SetInvincible(false);
            return NodeState::Success;
        }

        boss->SetSuperArmor(true);
        _targetClone = static_cast<BossClone*>(cloneActor);

        _bossStartPos = bb->BossTransform->GetPos();
        _cloneStartPos = _targetClone->GetPos();
        float groundY = _bossStartPos.y;

        // 1) 중앙 상공 정점 좌표 (CloneActivate와 동일한 높이)
        _apexPos = Vector(GWinSizeX * 0.5f, groundY - 260.0f);

        // 2) 중앙 착지 목표 좌표
        _landPos = Vector(GWinSizeX * 0.5f, groundY);

        // 무적 처리
        boss->SetInvincible(true);
        _targetClone->SetInvincible(true);

        // 서로 중앙을 마주보도록 방향 설정
        boss->SetLookDirX(_bossStartPos.x < _apexPos.x ? 1.0f : -1.0f);
        _targetClone->SetLookDirX(_cloneStartPos.x < _apexPos.x ? 1.0f : -1.0f);

        // 점프 애니메이션 재생
        if (bb->BossAnimation)
            bb->BossAnimation->Play(L"Jump");

        AnimatorComponent* cloneAnim = _targetClone->GetComponent<AnimatorComponent>();
        if (cloneAnim)
            cloneAnim->Play(L"Jump");

        _stepTimer = 0.0f;
        _step = DeactivateStep::JumpToCenter;
        return NodeState::Running;
    }

    _stepTimer += dt;

    // 1단계: 본체와 분신이 중앙 상공 정점으로 상승 (Ease-Out)
    if (_step == DeactivateStep::JumpToCenter)
    {
        const float duration = 0.5f;
        float t = min(_stepTimer / duration, 1.0f);

        float easedT = 1.0f - (1.0f - t) * (1.0f - t);
        Vector bossCurrentPos = Vector(lerp(_bossStartPos.x, _apexPos.x, easedT),
                                       lerp(_bossStartPos.y, _apexPos.y, easedT));
        boss->SetPos(bossCurrentPos);

        if (_targetClone)
        {
            Vector cloneCurrentPos = Vector(lerp(_cloneStartPos.x, _apexPos.x, easedT),
                                            lerp(_cloneStartPos.y, _apexPos.y, easedT));
            _targetClone->SetPos(cloneCurrentPos);
        }

        if (_stepTimer >= duration)
        {
            boss->SetPos(_apexPos);

            // SFX 재생
            SoundManager::GetInstance().PlaySFX(L"Gemini_Razer");

            // 분신 소멸
            if (_targetClone)
            {
                SceneManager::GetInstance().GetScene()->DeleteActor(_targetClone);
                _targetClone = nullptr;
            }
            bb->SetActor("BossClone", nullptr);

            // 정점에서 멈추지 않고 즉시 중앙 바닥으로 하강 착지 단계로 전환
            _stepTimer = 0.0f;
            _step = DeactivateStep::CenterLanding;
        }
        return NodeState::Running;
    }

    // 2단계: 중앙 바닥으로 수직 하강 착지 (Ease-In)
    if (_step == DeactivateStep::CenterLanding)
    {
        const float duration = 0.5f;
        float t = min(_stepTimer / duration, 1.0f);

        float easedT = t * t;
        Vector bossPos = Vector(lerp(_apexPos.x, _landPos.x, easedT),
                                lerp(_apexPos.y, _landPos.y, easedT));
        boss->SetPos(bossPos);

        if (_stepTimer >= duration)
        {
            boss->SetPos(_landPos);
            if (bb->BossRigidBody)
            {
                bb->BossRigidBody->SetVelocity(Vector(0.0f, 0.0f));
                bb->BossRigidBody->SetGrounded(true);
            }
            if (bb->BossAnimation)
                bb->BossAnimation->Play(L"Idle");

            boss->SetLookDirX(bb->DirXToPlayer);
            boss->SetInvincible(false);
            boss->SetSuperArmor(false);

            _step = DeactivateStep::None;
            _targetClone = nullptr;
            return NodeState::Success;
        }
        return NodeState::Running;
    }

    return NodeState::Running;
}

NodeState BTAction_Gemini_LaserMissile::Tick(Blackboard* bb)
{
    if (bb->OwnerBoss == nullptr || bb->BossAnimation == nullptr || bb->BossTransform == nullptr)
    {
        return NodeState::Failure;
    }

    Boss* boss = static_cast<Boss*>(bb->OwnerBoss);

    if (!_isAttackStarted)
    {
        boss->SetSuperArmor(true);
        boss->SetLookDirX(bb->DirXToPlayer);
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
        else
        {
            boss->SetSuperArmor(false);
            return NodeState::Failure;
        }
    }
    else
    {
        if (bb->BossAnimation->GetCurrentClipName() != L"Attack")
        {
            boss->SetSuperArmor(false);
            _isAttackStarted = false;
            _isAttackFinished = false;
            return NodeState::Failure;
        }

        if (_isAttackFinished)
        {
            boss->SetSuperArmor(false);
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
