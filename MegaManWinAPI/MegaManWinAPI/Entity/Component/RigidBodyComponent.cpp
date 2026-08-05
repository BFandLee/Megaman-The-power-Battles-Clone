#include "pch.h"
#include "RigidBodyComponent.h"
#include "Actor.h"

void RigidBodyComponent::Init()
{

}

void RigidBodyComponent::Update(float deltaTime)
{
    // 1. 중력 적용 (공중에 있을 때만 Y축 속도 증가)
    if (!_isGrounded)
    {
        _velocity.y += _gravity * deltaTime;
    }
    else
    {
        // 땅에 닿아있다면 Y축 속도를 0으로 초기화.
        if (_velocity.y > 0) _velocity.y = 0;

        // _velocity.x 값을 서서히 0에 가깝게 줄이는 로직(Lerp 등)을 추가하면 미끄러짐 효과가 생깁니다.
        _velocity.x = std::lerp(_velocity.x, 0, 0.1f);

    }
    // 2. 속도에 따른 실제 좌표 이동 (땅이든 공중이든 무조건 실행되어야 함!)
    Vector Pos = GetOwner()->GetPos();
    Vector CurrPos = Pos + _velocity * deltaTime;
    GetOwner()->SetPos(CurrPos);
}

void RigidBodyComponent::RenderUI()
{
	if (ImGui::TreeNode("RigidBody"))
	{
		ImGui::DragFloat2("Velocity", &_velocity.x, 0.3f);
		ImGui::DragFloat("Gravity", &_gravity);

		ImGui::TreePop();
	}
}

json RigidBodyComponent::ToJson()
{
	json j = Component::ToJson();

	j["VelocityX"] = _velocity.x;
	j["VelocityY"] = _velocity.y;
	j["Gravity"] = _gravity;

	return j;
}

void RigidBodyComponent::FromJson(const json& j)
{
	Super::FromJson(j);

	if (j.contains("VelocityX")) _velocity.x = j["VelocityX"];
	if (j.contains("VelocityY")) _velocity.y = j["VelocityY"];
	if (j.contains("Gravity"))   _gravity = j["Gravity"];
}
