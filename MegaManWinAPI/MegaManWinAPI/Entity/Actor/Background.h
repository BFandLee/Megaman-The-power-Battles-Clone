#pragma once
#include "Actor.h"

class Background : public Actor
{
	using Super = Actor;

public:
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Background; }
	virtual ActorType GetActorType() override { return ActorType::Background; }

private:
	class AnimatorComponent* _animator = nullptr;
};

