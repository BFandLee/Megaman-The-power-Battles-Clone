#include "pch.h"
#include "FSMComponent.h"
#include "State.h"

FSMComponent::FSMComponent() : Component("FSMComponent")
{
}

FSMComponent::~FSMComponent()
{
    // TODO: _states 맵을 순회하며 동적 할당된(new) State 객체들을 delete 해주는 메모리 관리 로직 작성
}

void FSMComponent::Init()
{
    // TODO: 초기 상태를 설정하는 로직
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
    // TODO: ImGui를 이용해 이동 속도 등 스탯을 조절하는 창 구현
}

json FSMComponent::ToJson()
{
    json j = Component::ToJson();
    // TODO: 컴포넌트의 수치들을 json 형식으로 저장(직렬화)
    return j;
}

void FSMComponent::FromJson(const json& j)
{
    Component::FromJson(j);
    // TODO: json 파일에서 수치들을 읽어와 복구(역직렬화)
}

void FSMComponent::AddState(const std::string& name, State* state)
{
    // TODO: 전달받은 state 포인터를 _states 맵에 추가
}

void FSMComponent::ChangeState(const std::string& name)
{
    // TODO: 현재 상태의 Exit() 호출 -> 상태 교체 -> 새 상태의 Enter() 호출 로직
}
