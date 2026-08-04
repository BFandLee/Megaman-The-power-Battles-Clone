#pragma once
#include "Actor.h"

class Player : public Actor
{
	using Super = Actor;

public:
	Player() : Actor("Player") {}
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	
	// 충돌 처리 함수
	virtual void OnStay(Actor* other, const HitResult& hit) override;
	virtual void OnExit(Actor* other) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Player; }
	virtual ActorType GetActorType() override { return ActorType::Player; }

private:
};
