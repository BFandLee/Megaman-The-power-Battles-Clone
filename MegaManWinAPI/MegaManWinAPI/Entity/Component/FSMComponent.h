#pragma once
#include "Component.h"
#include <map>
#include <string>

class State;

class FSMComponent : public Component
{
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

    // TODO: 이동 속도(MoveSpeed), 점프력 등 필요한 수치 데이터를 선언하세요.
private:
    float _movespeed = 0.0f;
    float _jumpforce = 0.0f;
};
