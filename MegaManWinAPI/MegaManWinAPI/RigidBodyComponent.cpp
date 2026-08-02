#include "pch.h"
#include "RigidBodyComponent.h"
#include "Actor.h"

void RigidBodyComponent::Init()
{

}

void RigidBodyComponent::Update(float deltaTime)
{
	if (!_isGrounded)
	{
		_velocity.y += _gravity * deltaTime;

		Vector Pos = GetOwner()->GetPos();
		Vector CurrPos = Pos + _velocity * deltaTime;
		GetOwner()->SetPos(CurrPos);
	}
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
