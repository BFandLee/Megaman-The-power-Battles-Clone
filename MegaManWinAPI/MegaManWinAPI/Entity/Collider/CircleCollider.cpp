#include "pch.h"
#include "CircleCollider.h"
#include "Actor.h"
#include "CollisionManager.h" // 디버그 렌더링 옵션 확인용 등 필요시 사용

CircleCollider::CircleCollider()
	: Collider(ColliderType::Circle)
{
}

CircleCollider::~CircleCollider()
{
	if (_brush)
		_brush->Release();
}

void CircleCollider::Update(float deltaTime)
{
	Collider::Update(deltaTime);
}

void CircleCollider::Render(ID2D1RenderTarget* renderTarget)
{
	if (!CollisionManager::GetInstance().GetDrawdebug()) return;

	if (_brush == nullptr)
	{
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &_brush);
	}
	Vector pos = GetColliderPos();

	D2D1_ELLIPSE ellipse;
	ellipse.point = D2D1::Point2F(pos.x, pos.y);	// 중심점
	ellipse.radiusX = _radius;
	ellipse.radiusY = _radius;

	renderTarget->DrawEllipse(ellipse, _brush, 1.0f);
	
}
