#pragma once
#include "Actor.h"

class PlayerBullet : public Actor
{
    using Super = Actor;
public:
    PlayerBullet() : Actor("PlayerBullet") {}
    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Bullet; } // ¶Ç´Â Bullet
    virtual ActorType GetActorType() override { return ActorType::PlayerBullet; }
    
    virtual void OnEnter(Actor* other, const HitResult& hit) override;

    void Reset(Vector startPos, Vector dir, ChargeLevel level);

private:
    Vector _dir;
    BulletState _state;
    ChargeLevel _chargeLevel = ChargeLevel::Normal;
    float _lifeTime = 0.0f;
};

