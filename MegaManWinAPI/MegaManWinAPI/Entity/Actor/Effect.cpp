#include "pch.h"
#include "Effect.h"
#include "AnimatorComponent.h"

void Effect::Init()
{
    Super::Init();
    _animator = AddComponent<AnimatorComponent>();
}

void Effect::Update(float deltaTime)
{
    Super::Update(deltaTime);

    if (_isPreview) return;

    if (_speed > 0.0f)
    {
        Vector currentPos = GetPos();
        currentPos = currentPos + (_dir * _speed * deltaTime);
        SetPos(currentPos);
    }

    _lifeTime += deltaTime;
    if (_lifeTime > _maxLifeTime)
    {
        Destroy();
    }
}

void Effect::PlayEffect(const std::wstring& animName, const std::wstring& jsonPath, Vector scale, bool isLoop)
{
    if (_animator == nullptr)
        return;

    this->SetScale(scale);

    _animator->LoadAnimationFromJson(animName, jsonPath);

    if (isLoop || _isPreview)
    {
        _animator->Play(animName);
    }

    else
    {
        // 1회 재생 완료 시 Destroy() 호출
        _animator->SetEndEvent(animName, [this]()
            {
                if (!_isPreview)
                {
                    Destroy();
                }
            });

        _animator->Play(animName);
    }
    
}