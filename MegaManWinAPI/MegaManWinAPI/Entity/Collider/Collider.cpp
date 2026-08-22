#include "pch.h"
#include "Collider.h"
#include "Actor.h"
#include "TransformComponent.h"

Collider::Collider(ColliderType type)
	: _colliderType(type), Component("Collider")
{
}

Collider::~Collider()
{
}

void Collider::Update(float deltaTime)
{
	// 기본 Collider는 매 프레임 특별히 갱신할 상태가 없다면 비워둡니다.
}

void Collider::Render(ID2D1RenderTarget* renderTarget)
{
	// 이 함수는 하위 클래스(Circle, Box)에서 오버라이딩하여 실제 충돌체 형태를 그릴 예정입니다.
}

Vector Collider::GetColliderPos()
{
    Vector finalOffset = _offset;
    TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();

    if (transform && transform->GetScale().x < 0)
    {
        finalOffset.x *= -1.0f;
    }

    return GetOwner()->GetPos() + finalOffset;
}
