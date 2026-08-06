#pragma once
#include "Component.h"
#include <map>
#include <string>

class State;

class FSMComponent : public Component
{
    using Super = Component;
private:
    std::map<std::string, State*> _states;
    State* _currentState = nullptr;

public:
    FSMComponent();
    virtual ~FSMComponent();

    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void RenderUI() override;
    virtual json ToJson() override;
    virtual void FromJson(const json& j) override;

    void AddState(const std::string& name, State* state);
    void ChangeState(const std::string& name);

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
    float _slideVelocity;
    float _friction;
    
};
