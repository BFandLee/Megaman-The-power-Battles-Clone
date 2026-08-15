#pragma once
#include "Actor.h"
class BossClone : public Actor
{
public:
	using Super =  Actor;
	BossClone();
	virtual ~BossClone();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void TakeDamage(float damage, float hitDirX = 0.0f) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Boss; }
	virtual ActorType GetActorType() override { return ActorType::BossClone; }

	virtual void OnStay(Actor* other, const HitResult& hit) override;
	virtual void OnExit(Actor* other) override;

	void SetOwner(class Boss* owner) { _ownerBoss = owner; }

private:
	class Boss* _ownerBoss = nullptr;
};

