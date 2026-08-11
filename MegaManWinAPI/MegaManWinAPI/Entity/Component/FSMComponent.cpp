#include "pch.h"
#include "FSMComponent.h"
#include "State.h"

FSMComponent::FSMComponent() : Component("FSMComponent")
{
}

FSMComponent::~FSMComponent()
{
    for (auto& iter : _states)
    {
        delete iter.second;
    }
}

void FSMComponent::Init()
{
    // 초기 상태를 설정하는 로직
    
}

void FSMComponent::Update(float deltaTime)
{
    if (_currentState != nullptr)
    {
        _currentState->Update(deltaTime);
    }
}

void FSMComponent::RenderUI()
{
  
}

json FSMComponent::ToJson()
{
    json j;
    return j;
}

void FSMComponent::FromJson(const json& j)
{

    
}
    
void FSMComponent::AddState(const std::string& name, State* state)
{
    _states.insert({ name, state });
}

void FSMComponent::ChangeState(const std::string& name)
{
    
    // map에서 stateName 키가 존재하는지 찾기
    auto it = _states.find(name);

    // 찾지 못했다면 함수 종료
    if (it == _states.end())
    {
        return;
    }

    if (_currentState != nullptr)
    {
        _currentState->Exit();
    }
    _currentState = it->second;
    _currentState->Enter();
}
