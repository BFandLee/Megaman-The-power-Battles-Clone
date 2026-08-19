#pragma once
#include "Actor.h"

class AnimatorComponent;

class Effect : public Actor
{
    using Super = Actor;

public:
    Effect() : Actor("EffectActor") {}
    virtual ~Effect() {}

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    virtual RenderLayer GetRenderLayer() override { return RenderLayer::Effect; }
    virtual ActorType GetActorType() override { return ActorType::Effect; }

    // 애니메이션 등록 및 재생 (재생 완료 시 자동 소멸)
    void PlayEffect(const std::wstring& animName, const std::wstring& jsonPath, Vector scale = Vector(2.0f, 2.0f), bool isLoop = false);

    void SetPreviewMode(bool isPreview) { _isPreview = isPreview; }
    void SetMoveInfo(const Vector& dir, float speed) { _dir = dir; _speed = speed; }
private:
    AnimatorComponent* _animator = nullptr;
    float _lifeTime = 0.0f;
    float _maxLifeTime = 5.0f; // 안전 타이머 (콜백 미발생 시 자동 정리)
    bool _isPreview = false;
    Vector _dir = Vector(0.0f, 0.0f);
    float _speed = 0.0f;
};