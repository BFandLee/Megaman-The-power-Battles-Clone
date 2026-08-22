#pragma once
#include "Actor.h"

class WallActor : public Actor
{
	using Super = Actor;
public:
	WallActor() : Actor("WallActor") {}
	virtual ~WallActor();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	
	virtual RenderLayer GetRenderLayer() override;
	virtual ActorType GetActorType() override;

	void SetActorType(ActorType type) { _actorType = type; }

	// 벽의 크기를 설정합니다.
	void SetSize(float width, float height);

private:
	ActorType _actorType = ActorType::WALL;
	class BoxCollider* _boxCollider = nullptr;
};
