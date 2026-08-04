#include "pch.h"
#include "PlayerBullet.h"
#include "CircleCollider.h"
#include "AnimatorComponent.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
void PlayerBullet::Init()
{
    Super::Init();
   CircleCollider* collider = AddComponent<CircleCollider>();

    AnimatorComponent* animator = AddComponent<AnimatorComponent>();
    animator->LoadAnimationFromJson(L"Attack", L"../Resources/sprites/AttackEffect/Animation/Attack.json");
    animator->Play(L"Attack");

    this->SetScale(Vector(2.0f, 2.0f));
    collider->SetRadius(15.0f);
}

void PlayerBullet::Update(float deltaTime)
{
    Super::Update(deltaTime);
    Vector nextpos = GetPos() + (_dir * _speed * deltaTime);
    this->SetPos(nextpos);

    _lifeTime += deltaTime;

    if (_lifeTime > _maxLifeTime)
    {
        Destroy();
    }
}

void PlayerBullet::Reset(Vector startPos, Vector dir)
{
    this->SetPos(startPos);
    _dir = dir;
    _lifeTime = 0;

}
