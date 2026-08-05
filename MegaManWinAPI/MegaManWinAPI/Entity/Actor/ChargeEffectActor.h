#pragma once
#include "Actor.h"

class AnimatorComponent;

class ChargeEffectActor : public Actor
{
    using Super = Actor;

public:
    ChargeEffectActor();
    virtual ~ChargeEffectActor();

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    // TODO(USER): RenderLayer와 ActorType을 알맞게 설정해 주세요. (미구현 시 빌드 에러 날 수 있음)
    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Effect; }
    virtual ActorType GetActorType() override { return ActorType::Effect; }

    // 차지 레벨(Mid, Max)에 따라 이펙트 애니메이션을 변경할 수 있는 함수
    void SetChargeLevel(int level);

private:
    AnimatorComponent* _animator = nullptr;
};
