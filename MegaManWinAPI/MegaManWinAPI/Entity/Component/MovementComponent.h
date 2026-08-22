#pragma once
#include "Component.h"

class MovementComponent : public Component
{
    using Super = Component;
public:
    MovementComponent() : Super("MovementComponent") {}
    virtual ~MovementComponent() {}

    virtual void Update(float deltaTime) override;
    virtual void RenderUI() override; // 이동/물리 변수 실시간 모니터링 및 수정용 ImGui 렌더링

    virtual json ToJson() override;
    virtual void FromJson(const json& j) override;

    float GetMoveSpeed() { return _movespeed; }
    float GetJumpForce() { return _jumpforce; }
    float GetAccel() { return _currentaccel; }
    float GetSlideVelcoity() { return _slideVelocity; }
    float GetFriction() { return _friction; }

private:
    float _movespeed = 0.0f;
    float _jumpforce = 0.0f;
    float _accel = 10.0f;
    float _currentaccel = _accel * 1000.0f;

    // 슬라이딩
    float _slideVelocity = 0.0f;
    float _friction = 0.0f;
};

