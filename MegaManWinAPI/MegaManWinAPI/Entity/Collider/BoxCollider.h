#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	float GetWidth() const { return _width; }
	float GetHeight() const { return _height; }
	
	void SetSize(float width, float height)
	{
		_width = width;
		_height = height;
	}

private:
	ID2D1SolidColorBrush* _brush = nullptr;
	float _width = 0.0f;
	float _height = 0.0f;
};
