#pragma once
#include "Actor.h"

class Ground : public Actor
{
	using Super = Actor;

public:
	Ground() : Actor("Ground") {}
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	// Ground는 일단 Background 레이어에 그리지만, Background Actor보다 늦게 생성(AddActor)하면 위에 그려집니다.
	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Background; }
	virtual ActorType GetActorType() override { return ActorType::Background; } 

private:
	float _weight = 25.0f;		// ground의 위치를 맞추기 위한 가중치
};
