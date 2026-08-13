#pragma once
#include "Actor.h"
class BossMissile : public Actor
{
    using Super = Actor;
public:
    BossMissile() : Actor("BossMissile") {}
    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Bullet; }
    virtual ActorType GetActorType() override { return ActorType::BossBullet; }

    virtual void OnEnter(Actor* other, const HitResult& hit) override;

    void Fire(Vector startPos, Vector dir, float speed, float damage);

private:
    Vector _dir;
    BulletState _state;
    float _lifeTime = 0.0f;
};

