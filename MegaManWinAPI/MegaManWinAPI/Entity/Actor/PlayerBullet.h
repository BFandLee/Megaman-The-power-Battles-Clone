#pragma once
#include "Actor.h"

struct BulletState
{
    float damage;
    float speed;
    bool isPiercing;    // 관통 여부
    float maxLifeTIme = 1.0f;
};

class PlayerBullet : public Actor
{
    using Super = Actor;
public:
    PlayerBullet() : Actor("PlayerBullet") {}
    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Bullet; } // 또는 Bullet
    virtual ActorType GetActorType() override { return ActorType::PlayerBullet; }
    
    virtual void OnEnter(Actor* other, const HitResult& hit) override;

    void Reset(Vector startPos, Vector dir, ChargeLevel level);

private:
    Vector _dir;
    BulletState _state;
    float _lifeTime = 0.0f;
};

