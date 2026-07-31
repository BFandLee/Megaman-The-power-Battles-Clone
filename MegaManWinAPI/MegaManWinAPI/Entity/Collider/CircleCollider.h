#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider();
	virtual ~CircleCollider();

	virtual void Update(float deltaTime) override;
	
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	float GetRadius() const { return _radius; }
	void SetRadius(float radius) { _radius = radius; }

private:
	float _radius = 0.0f;
	ID2D1SolidColorBrush* _brush = nullptr;
};
