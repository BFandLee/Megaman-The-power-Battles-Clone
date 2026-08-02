#pragma once
#include "Component.h"
class RigidBodyComponent : public Component
{
    using Super = Component;

public:
    RigidBodyComponent() : Super("RigidBodyComponent") {}
    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void RenderUI();

    // 속도 관련
    void SetVelocity(Vector velocity) { _velocity = velocity; }
    void AddVelocity(Vector velocity) { _velocity += velocity; }
    Vector GetVelocity() const { return _velocity; }

    // 바닥 충돌 상태 관리
    void SetGrounded(bool grounded) { _isGrounded = grounded; }
    bool IsGrounded() const { return _isGrounded; }

    // 중력 가속도 설정
    void SetGravity(float gravity) { _gravity = gravity; }

    // GUI 연결(Json)
    virtual json ToJson() override;
    virtual void FromJson(const json& j) override;

private:
    Vector _velocity = { 0.f, 0.f };
    float _gravity = 980.f;      // 픽셀 기준의 중력 가속도 (적절히 조절 가능)
    bool _isGrounded = false;    // 현재 바닥에 닿아 있는가?
};

