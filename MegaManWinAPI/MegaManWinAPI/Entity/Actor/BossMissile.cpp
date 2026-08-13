#include "pch.h"
#include "BossMissile.h"
#include "BoxCollider.h"
#include "AnimatorComponent.h"

void BossMissile::Init()
{
    Super::Init();
    BoxCollider* collider = AddComponent<BoxCollider>();

    AnimatorComponent* animator = AddComponent<AnimatorComponent>();
    animator->LoadAnimationFromJson(L"Attack", L"../Resources/sprites/AttackEffect/Boss/Base.json");
    animator->Play(L"Attack");

    this->SetScale(Vector(2.0f, 2.0f));
    collider->SetSize(30.0f, 5.0f);
    collider->SetOffset(Vector(0.0f, 0.0f));
}

void BossMissile::Update(float deltaTime)
{
    Super::Update(deltaTime);
    Vector nextpos = GetPos() + (_dir * _state.speed * deltaTime);
    this->SetPos(nextpos);

    _lifeTime += deltaTime;

    if (_lifeTime > _state.maxLifeTIme)
    {
        Destroy();
    }
}

void BossMissile::OnEnter(Actor* other, const HitResult& hit)
{
    bool isEnemy = (other->GetActorType() == ActorType::Player);

    if (isEnemy)
    {
        other->TakeDamage(_state.damage, _dir.x);
        Destroy();
    }
}

void BossMissile::Fire(Vector startPos, Vector dir, float speed, float damage)
{
    this->SetPos(startPos);
    _dir = dir;
    _state.speed = speed;
    _state.damage = damage;
}
