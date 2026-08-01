#include "pch.h"
#include "BoxCollider.h"
#include "Actor.h"
#include "CollisionManager.h"

BoxCollider::BoxCollider()
	: Collider(ColliderType::Box)
{
}

BoxCollider::~BoxCollider()
{
	if (_brush)
		_brush->Release();
}

void BoxCollider::Update(float deltaTime)
{
	Collider::Update(deltaTime);
}

void BoxCollider::Render(ID2D1RenderTarget* renderTarget)
{
	if (!CollisionManager::GetInstance().GetDrawdebug())
	{
		return;
	}


	if (_brush == nullptr)
	{
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &_brush);
	}
	Vector pos = GetColliderPos();

	D2D1_RECT_F rect;
	rect.left = pos.x - _width / 2.0f;
	rect.right = pos.x + _width / 2.0f;
	rect.top = pos.y - _height / 2.0f;
	rect.bottom = pos.y + _height / 2.0f;

	// DrawRectangle 호출 로직 작성
	renderTarget->DrawRectangle(rect, _brush, 1.0f);
}

void BoxCollider::RenderUI()
{
	if (ImGui::TreeNode("BoxCollider"))
	{
		ImGui::DragFloat("Width", &_width);
		ImGui::DragFloat("Height", &_height);
		ImGui::DragFloat2("Offset", &_offset.x, 0.3f);

		ImGui::TreePop();
	}
	
}

json BoxCollider::ToJson()
{
	json j = Component::ToJson();

	j["Width"] = _width;
	j["Height"] = _height;

	return j;
}

void BoxCollider::FromJson(const json& j)
{
	Super::FromJson(j);

	if (j.contains("Width")) _width = j["Width"];
	if (j.contains("Height")) _height = j["Height"];
}
