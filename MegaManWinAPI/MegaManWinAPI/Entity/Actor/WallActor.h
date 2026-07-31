#pragma once
#include "Actor.h"

class WallActor : public Actor
{
	using Super = Actor;
public:
	WallActor();
	virtual ~WallActor();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	virtual RenderLayer GetRenderLayer() override;
	virtual ActorType GetActorType() override;

	// 벽의 크기를 설정합니다.
	void SetSize(float width, float height);

private:
	
	class BoxCollider* _boxCollider = nullptr;
};
