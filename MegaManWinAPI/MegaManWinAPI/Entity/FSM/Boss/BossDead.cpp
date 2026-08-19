#include "pch.h"
#include "BossDead.h"
#include "Boss.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "BoxCollider.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Effect.h"
#include "SoundManager.h"
#include "CollisionManager.h"

BossDead::BossDead(FSMComponent* fsm) : State(fsm)
{
}

BossDead::~BossDead()
{
}

void BossDead::Enter()
{
    _deadTimer = 0.0f;
    _isExploded = false;
    _boss = static_cast<Boss*>(m_pOwnerFSM->GetOwner());

    if (_boss == nullptr) return;

    // 1. 사망 사운드 재생
    SoundManager::GetInstance().PlayBGM(L"geminiman_death", false);

    // 2. 피격(Hit) 애니메이션 고정 및 무적화
    _boss->SetInvincible(true);
    AnimatorComponent* anim = _boss->GetComponent<AnimatorComponent>();
    if (anim != nullptr)
    {
        anim->Play(L"Hit");
    }

    RigidBodyComponent* rigid = _boss->GetComponent<RigidBodyComponent>();
    if (rigid != nullptr)
    {
        rigid->SetVelocity(Vector(0.0f, 0.0f)); // 이동 및 낙하 속도 제거
        rigid->SetGravity(0.0f);                // 중력 제거
    }

    // 3. 충돌체 비활성화 (플레이어 충돌 및 피격 방지)
    BoxCollider* collider = _boss->GetComponent<BoxCollider>();
    if (collider != nullptr)
    {
        CollisionManager::GetInstance().RemoveActor(_boss);
    }

    // 4. 필드에 남아있는 분신(BossClone) 소멸
    Scene* scene = SceneManager::GetInstance().GetScene();
    if (scene != nullptr)
    {
        Actor* clone = scene->FindActorByType(ActorType::BossClone);
        if (clone != nullptr)
        {
            clone->Destroy();
        }
    }
}

void BossDead::Update(float deltaTime)
{
    if (_boss == nullptr || _isExploded) return;

    _deadTimer += deltaTime;

    // 1.5초 경과 시 8방향 폭발 연출 및 보스 소멸
    if (_deadTimer >= _maxDeadTime)
    {
        _isExploded = true;

        Scene* scene = SceneManager::GetInstance().GetScene();
        if (scene != nullptr)
        {
            Vector bossPos = _boss->GetPos();
            const float PI = 3.1415926535f;
            const int count = 8;
            const float speed = 250.0f; // 뻗어나가는 속도 (px/s)

            // 8방향(45도 간격)으로 이펙트 생성 및 방출
            for (int i = 0; i < count; ++i)
            {
                float angle = i * (2.0f * PI / (float)count);
                Vector dir(cosf(angle), sinf(angle));

                Effect* effect = new Effect();
                effect->Init();
                effect->SetPos(bossPos);
                effect->SetMoveInfo(dir, speed);
                effect->PlayEffect(L"Effect", L"../Resources/sprites/BossDead/Animation/Effect.json", Vector(2.0f, 2.0f), false);

                scene->AddActor(effect);
            }
        }

        // 보스 소멸
        _boss->Destroy();
    }
}

void BossDead::Exit()
{
}