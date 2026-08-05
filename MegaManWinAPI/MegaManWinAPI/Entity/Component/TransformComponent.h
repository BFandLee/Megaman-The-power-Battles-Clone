#pragma once
#include "Component.h"
class TransformComponent : public Component
{
	using Super = Component;
public:
	TransformComponent() : Component("TransformComponent") {}
	virtual ~TransformComponent() = default;

	// Pos
	Vector GetPos() const { return _pos; }
	void SetPos(Vector pos) { _pos = pos; }

	// Scale
	Vector GetScale() const { return _Scale; }
	void SetScale(Vector Scale) { _Scale = Scale; }
	
	// Rotation
	float GetRotation() const { return _rotation; }
	void SetRotation(float rotation) { _rotation = rotation; }

public:
	virtual void RenderUI() override;
	virtual json ToJson() override;
	virtual void FromJson(const json& j) override;

private:
	Vector _pos = Vector(0.0f, 0.0f);
	Vector _Scale = Vector(1.0f, 1.0f);
	float _rotation = 0.0f;
};

