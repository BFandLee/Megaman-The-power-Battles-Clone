#include "pch.h"
#include "MovementComponent.h"

void MovementComponent::Update(float deltaTime)
{
}

void MovementComponent::RenderUI()
{
    if (ImGui::TreeNode("Movement"))
    {
        // 속도 조절
        ImGui::DragFloat("MoveSpeed", &_movespeed);

        // 점프 크기 조절
        ImGui::DragFloat("JumpForce", &_jumpforce);

        // 가속도
        ImGui::DragFloat("Accel", &_accel);

        // 슬라이딩 관련 
        if (ImGui::TreeNode("Slide"))
        {
            ImGui::DragFloat("Velocity", &_slideVelocity);

            ImGui::DragFloat("Friction", &_friction);

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

json MovementComponent::ToJson()
{
    json j = Super::ToJson();

    j["MoveSpeed"] = _movespeed;
    j["JumpForce"] = _jumpforce;
    j["Accel"] = _accel;
    j["Velocity"] = _slideVelocity;
    j["Friction"] = _friction;


    return j;
}

void MovementComponent::FromJson(const json& j)
{
    Super::FromJson(j);

    if (j.contains("MoveSpeed")) _movespeed = j["MoveSpeed"];
    if (j.contains("JumpForce")) _jumpforce = j["JumpForce"];
    if (j.contains("Accel")) _accel = j["Accel"];
    if (j.contains("Velocity")) _slideVelocity = j["Velocity"];
    if (j.contains("Friction")) _friction = j["Friction"];
}
