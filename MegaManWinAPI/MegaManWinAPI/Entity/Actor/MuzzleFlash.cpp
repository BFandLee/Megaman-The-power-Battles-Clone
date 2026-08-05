#include "pch.h"
#include "MuzzleFlash.h"
#include "AnimatorComponent.h"

void MuzzleFlash::Init()
{
    Super::Init();
    // 1. 애니메이터 부착
    _animator = AddComponent<AnimatorComponent>();

    // 기 모으기 이펙트 전용 애니메이션 추가하기
    _animator->LoadAnimationFromJson(L"Firing_Effect", L"../Resources/sprites/AttackEffect/Animation/Firing_Effect.json");

    // 초기에는 Mid 애니메이션 재생
    _animator->Play(L"Firing_Effect");
}

void MuzzleFlash::Update(float deltaTime)
{
    Super::Update(deltaTime);
    _lifeTime += deltaTime;

    // _maxLifeTime --> json에 모든 dursion 시간을 합한 시간이 되어야함
    if (_lifeTime > _maxLifeTime)
    {
        Destroy();
    }
}
