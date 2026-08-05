#include "pch.h"
#include "ChargeEffectActor.h"
#include "AnimatorComponent.h"
#include "TimeManager.h"
#include "TransformComponent.h"

ChargeEffectActor::ChargeEffectActor() : Actor("ChargeEffectActor")
{
}

ChargeEffectActor::~ChargeEffectActor()
{
}

void ChargeEffectActor::Init()
{
    Super::Init();

    // 1. 애니메이터 부착
    _animator = AddComponent<AnimatorComponent>();

    _animator->LoadAnimationFromJson(L"Mid_Charge_Shot", L"../Resources/sprites/Effect/Mid_Charge_Shot.json");
    _animator->LoadAnimationFromJson(L"Max_Charge_Shot", L"../Resources/sprites/Effect/Max_Charge_Shot.json");
    
    // 초기에는 Mid 애니메이션 재생
    _animator->Play(L"Mid_Charge_Shot");
}

void ChargeEffectActor::Update(float deltaTime)
{
    Super::Update(deltaTime);
}

void ChargeEffectActor::SetChargeLevel(int level)
{
    // TODO(USER): level 매개변수에 따라 (1 = Mid, 2 = Max 등)
    // _animator->Play() 함수를 호출하여 이펙트를 변경해 보세요!
    switch (level)
    {
        case (int32)ChargeLevel::Mid:
        {
            _animator->Play(L"Mid_Charge_Shot");
            break;
        }
        case (int32)ChargeLevel::Max:
        {
            _animator->Play(L"Max_Charge_shot");
            break;
        }
    default:
        break;
    }
}
