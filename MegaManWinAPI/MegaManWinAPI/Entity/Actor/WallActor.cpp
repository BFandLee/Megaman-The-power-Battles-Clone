#include "pch.h"
#include "WallActor.h"
#include "CollisionManager.h"
#include "BoxCollider.h"


WallActor::~WallActor()
{
}

void WallActor::Init()
{
	Super::Init();

	_boxCollider = AddComponent<BoxCollider>();
}

void WallActor::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void WallActor::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}


RenderLayer WallActor::GetRenderLayer()
{
	// 보이지 않는 벽이므로 렌더 레이어는 사실상 무관합니다.
	// 하지만 디버그 렌더링을 위해 Layer를 지정해 줍니다.
	return RenderLayer::WALL; // 혹은 환경 오브젝트에 맞는 Layer
}

ActorType WallActor::GetActorType()
{
	return ActorType::WALL;
}

void WallActor::SetSize(float width, float height)
{
	if (_boxCollider)
	{
		_boxCollider->SetSize(width, height);
	}
}
