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
    std::string _currentStateName = "";

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
    const std::string& GetCurrentStateName() const { return _currentStateName; }

    
    
};
