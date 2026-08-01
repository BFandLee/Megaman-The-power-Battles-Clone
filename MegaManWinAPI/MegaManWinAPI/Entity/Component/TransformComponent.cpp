#include "pch.h"
#include "TransformComponent.h"

void TransformComponent::RenderUI()
{
	if (ImGui::TreeNode("Transform"))
	{
		// Position 조절(마지막 인자 : 드래그 시 변하는 속도)
		ImGui::DragFloat2("Position", &_pos.x, 1.0f);

		// Scale 조절
		ImGui::DragFloat2("Scale", &_Scale.x, 0.1f);

		// Rotation 조절
		ImGui::DragFloat("Rotation", & _rotation);

		ImGui::TreePop();
	}
}

json TransformComponent::ToJson()
{
	json j = Super::ToJson();

	// 고유한 데이터 추가
	j["PosX"] = _pos.x;
	j["PosY"] = _pos.y;

	j["ScaleX"] = _Scale.x;
	j["ScaleY"] = _Scale.y;

	j["Rotation"] = _rotation;
	
	return j;


}

void TransformComponent::FromJson(const json& j)
{
	Super::FromJson(j);

	if (j.contains("PosX")) _pos.x = j["PosX"];
	if (j.contains("PosY")) _pos.y = j["PosY"];

	if (j.contains("ScaleX")) _Scale.x = j["ScaleX"];
	if (j.contains("ScaleY")) _Scale.y = j["ScaleY"];

	if (j.contains("Rotation")) _rotation = j["Rotation"];
}
