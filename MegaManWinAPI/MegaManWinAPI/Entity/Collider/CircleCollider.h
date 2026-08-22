#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
	using Super = Collider;
public:
	CircleCollider();
	virtual ~CircleCollider();

	virtual void Update(float deltaTime) override;
	
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void RenderUI() override;

	float GetRadius() const { return _radius; }
	void SetRadius(float radius) { _radius = radius; }

	virtual json ToJson() override;
	virtual void FromJson(const json& j) override;

private:
	float _radius = 0.0f;
	ID2D1SolidColorBrush* _brush = nullptr;
};
