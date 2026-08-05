#pragma once
#include "Actor.h"

struct BulletState
{
    float damage;
    float speed;
    bool isPiercing;
};

class PlayerBullet : public Actor
{
    using Super = Actor;
public:
    PlayerBullet() : Actor("PlayerBullet") {}
    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Bullet; } // ¶Ç´Â Bullet
    virtual ActorType GetActorType() override { return ActorType::PlayerBullet; }
    
    void Reset(Vector startPos, Vector dir, ChargeLevel level);

private:
    Vector _dir;
    float _speed = 500.0f;
    float _damage = 0.0f;
    float _lifeTime = 0.0f;
    float _maxLifeTime = 1.0f;
    bool _isPiercing = false;
};

