#include "pch.h"
#include "FSMComponent.h"
#include "State.h"

FSMComponent::FSMComponent() : Component("FSMComponent")
{
}

FSMComponent::~FSMComponent()
{
    // TODO: _states 맵을 순회하며 동적 할당된(new) State 객체들을 delete 해주는 메모리 관리 로직 작성
    for (auto& iter : _states)
    {
        delete iter.second;
    }
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
    if (ImGui::TreeNode("FSM"))
    {
        // 속도 조절
        ImGui::DragFloat("MoveSpeed", &_movespeed);

        // 점프 크기 조절
        ImGui::DragFloat("JumpForce",&_jumpforce);

        ImGui::TreePop();
    }
}

json FSMComponent::ToJson()
{
    json j = Super::ToJson();
    // TODO: 컴포넌트의 수치들을 json 형식으로 저장(직렬화)
    j["MoveSpeed"] = _movespeed;
    j["JumpForce"] = _jumpforce;


    return j;
}

void FSMComponent::FromJson(const json& j)
{
    Super::FromJson(j);
    // TODO: json 파일에서 수치들을 읽어와 복구(역직렬화)
    if (j.contains("MoveSpeed")) _movespeed = j["MoveSpeed"];
    if (j.contains("JumpForce")) _jumpforce = j["JumpForce"];
    
}
    
void FSMComponent::AddState(const std::string& name, State* state)
{
    // TODO: 전달받은 state 포인터를 _states 맵에 추가
    _states.insert({ name, state });
}

void FSMComponent::ChangeState(const std::string& name)
{
    // TODO: 현재 상태의 Exit() 호출 -> 상태 교체 -> 새 상태의 Enter() 호출 로직
    
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
