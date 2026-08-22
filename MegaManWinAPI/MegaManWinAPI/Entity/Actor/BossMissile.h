#pragma once
#include "Actor.h"
class BossMissile : public Actor
{
    using Super = Actor;
public:
    BossMissile() : Actor("BossMissile") {}
    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void RenderUI() override;
    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Bullet; }
    virtual ActorType GetActorType() override { return ActorType::BossBullet; }

    virtual void OnEnter(Actor* other, const HitResult& hit) override;

    void Fire(Vector startPos, Vector dir, MissileType type = MissileType::Base, 
        int BounceCount = 0, Actor* ignoreActor = nullptr);
    
    void LoadDataFromJson(MissileType type);
    void SaveDataToJson(MissileType type);
private:
    class AnimatorComponent* _animator = nullptr;
    class BoxCollider* _collider = nullptr;
    class TransformComponent* _transform = nullptr;
    Actor* _ignoreActor = nullptr;

    Vector _dir;
    BulletState _state;
    MissileType _type;

    float _lifeTime = 0.0f;
    int _bounceCount = 0;
    int _maxBounceCount = 5;

    bool _hasBounced = false;
    bool _isDestoryed = false;
    
    const std::wstring _dataPath = L"Resources/Data/BossBulletData.json";
};

