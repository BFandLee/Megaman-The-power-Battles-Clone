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

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Enemy; }
	virtual ActorType GetActorType() override { return ActorType::Enemy; }
};
