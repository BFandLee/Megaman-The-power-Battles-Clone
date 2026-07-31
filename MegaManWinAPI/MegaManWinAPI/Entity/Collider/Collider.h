#pragma once
#include "Component.h"

// 충돌 결과 구조체 (추후 방향, 깊이 등 필요시 확장 가능)
struct HitResult
{
	class Actor* actor = nullptr;
};

class Collider : public Component
{
public:
	Collider(ColliderType type);
	virtual ~Collider();

	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	// Q1. Actor의 위치(GetPos)에서 얼마나 떨어져 있는지 설정하는 offset 관련 함수들을 구현해보세요.
	Vector GetOffset() const { return _offset; }
	void SetOffset(Vector offset) { _offset = offset; }

	// Q2. 실제 충돌체의 중심 위치 = Actor의 위치 + Offset 입니다. 이를 반환하는 함수를 완성해보세요.
	Vector GetColliderPos();

	ColliderType GetColliderType() const { return _colliderType; }

protected:
	ColliderType _colliderType;
	Vector _offset; // Actor 중심으로부터 떨어진 거리
};
