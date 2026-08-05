#pragma once
#include "Actor.h"

class AnimatorComponent;

class MuzzleFlash : public Actor
{
    using Super = Actor;

public:
    MuzzleFlash() : Actor("MuzzleFlash") {}
    virtual ~MuzzleFlash() {};

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Effect; }
    virtual ActorType GetActorType() override { return ActorType::Effect; }

    // 차지 레벨(Mid, Max)에 따라 이펙트 애니메이션을 변경할 수 있는 함수
    void SetChargeLevel(int level);

private:
    AnimatorComponent* _animator = nullptr;
    float _lifeTime = 0.0f;
    float _maxLifeTime = 2.0f;
};
