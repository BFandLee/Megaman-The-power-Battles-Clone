#include "pch.h"
#include "Actor.h"
#include "Game.h"
#include "Scene.h"
#include "Component.h"
#include "SceneManager.h"
// #include "Collider.h"
#include "TransformComponent.h"

Actor::~Actor()
{
	// new Component 들.. 메모리 해제.
	for (auto component : _components)
	{
		delete component;
	}
	_components.clear();
}

void Actor::Init()
{
	_transform = AddComponent<TransformComponent>();
}

// Actor 파괴(삭제) 싶으면, 무조건 Scene에 예약을 걸어서 처리한다.
void Actor::Destroy()
{
	// 1번 방식
	//_pendingKill = true; // 삭제 예약 상태 flag 추가해도 된다.
	
	// 2번 방식
	SceneManager::GetInstance().GetScene()->DeleteActor(this);
}

void Actor::Update(float deltaTime)
{
	for (auto component : _components)
	{
		component->Update(deltaTime);
	}
}

void Actor::Render(ID2D1RenderTarget* renderTarget)
{
	for (auto component : _components)
	{
		component->Render(renderTarget, GetPos());
	}
}

Vector Actor::GetPos() const
{
	return _transform ? _transform->GetPos() : Vector(0, 0);
}
// 위치가 변경되었으니 grid 갱신도 같이 해주자.
void Actor::SetPos(Vector pos)
{
	if (_transform) _transform->SetPos(pos);
}

Vector Actor::GetScale() const
{
	return _transform ? _transform->GetScale() : Vector(1.0f, 1.0f);
}
void Actor::SetScale(Vector scale)
{
	if (_transform) _transform->SetScale(scale);
}

//void Actor::cacheCollider(Component* component)
//{
//	Collider* collider = dynamic_cast<Collider*>(component);
//	if (collider)
//	{
//		_collider = collider;	// 한번 캐싱해둔다.
//	}
//}
