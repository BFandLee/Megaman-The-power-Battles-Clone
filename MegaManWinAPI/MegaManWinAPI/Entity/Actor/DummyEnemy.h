#pragma once
#include "Actor.h"

class DummyEnemy : public Actor
{
	using Super = Actor;
public:
	DummyEnemy();
	virtual ~DummyEnemy();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void TakeDamage(float damage, float hitDirX = 0.0f) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Enemy; }
	virtual ActorType GetActorType() override { return ActorType::Enemy; }

private:
	bool _isHit = false;
	float _hitDuration = 0.0f;
};
