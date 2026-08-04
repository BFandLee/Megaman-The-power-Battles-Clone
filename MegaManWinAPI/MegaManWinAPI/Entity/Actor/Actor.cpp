#include "pch.h"
#include "Actor.h"
#include "Game.h"
#include "Scene.h"
#include "Component.h"
#include "SceneManager.h"
#include "Collider.h"
#include "TransformComponent.h"
#include "CollisionManager.h"

Actor::Actor(string name) : _name(name)
{

}

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

	CollisionManager::GetInstance().RemoveActor(this);
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
		component->Render(renderTarget);
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

void Actor::cacheCollider(Component* component)
{
	Collider* collider = dynamic_cast<Collider*>(component);
	if (collider)
	{
		_collider = collider;	// 한번 캐싱해둔다.
	}
}

void Actor::RenderUI()
{
	ImGui::PushID(this);

	if (ImGui::TreeNode(_name.c_str()))
	{
		for (auto& component : _components)
		{
			component->RenderUI();
		}

		ImGui::TreePop();
	}

	ImGui::PopID();
}

json Actor::ToJson()
{
	json j;
	j["name"] = _name;

	// 내가 가진 컴포넌트들을 배열로 묶어서 저장
	json componentsArray = json::array();

	for (auto comp : _components)
	{
		componentsArray.push_back(comp->ToJson());
	}

	j["Components"] = componentsArray;
	return j;
}

void Actor::FromJson(const json& j)
{
	if (j.contains("name")) _name = j["name"];

	// 저장된 컴포넌트 배열을 읽어와서 덮어씁니다.
	int index = 0;
	for (const auto& compJson : j["Components"])
	{
		// 엑터가 init() 되면서 생성해둔 컴포넌트 개수를 초과하지 않는지 안전검사
		if (index < _components.size())
		{
			_components[index]->FromJson(compJson);
		}
		index++;
	}
}