#include "pch.h"
#include "Blackboard.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "AnimatorComponent.h"
#include "SceneManager.h"
#include "Scene.h"

void Blackboard::Init(Actor* owner)
{
	OwnerBoss = owner;
	if (OwnerBoss != nullptr)
	{
		BossTransform = owner->GetComponent<TransformComponent>();
		BossRigidBody = owner->GetComponent<RigidBodyComponent>();
		BossAnimation = owner->GetComponent<AnimatorComponent>();
	}
}
void Blackboard::Update(float deltaTime)
{
	Actor* player = SceneManager::GetInstance().GetScene()->FindActorByType(ActorType::Player);

	// 1. 플레이어가 아직 없으면 씬에서 탐색하여 캐싱
		
	if (player != nullptr)
	{
		TargetPlayer = player;
		// TODO: player로부터 PlayerTransform과 PlayerRigidBody를 GetComponent하여 캐싱하세요.
		PlayerTransform = player->GetComponent<TransformComponent>();
		PlayerRigidBody = player->GetComponent<RigidBodyComponent>();
	}
	else
	{
		TargetPlayer = nullptr;
		PlayerTransform = nullptr;
		PlayerRigidBody = nullptr;
	}
	
	// 2. 플레이어와 보스가 모두 존재할 경우 상대 방향(DirXToPlayer) 자동 계산
	if (PlayerTransform != nullptr && BossTransform != nullptr)
	{
		float playerX = PlayerTransform->GetPos().x;
		float bossX = BossTransform->GetPos().x;
		DirXToPlayer = (playerX > bossX) ? 1.0f : -1.0f;
	}

	Actor* clone = SceneManager::GetInstance().GetScene()->FindActorByType(ActorType::BossClone);
	BossClone = clone;
}

void Blackboard::SetFloat(const std::string& key, float value)
{
	FloatData[key] = value;
}

bool Blackboard::GetFloat(const std::string& key, float& outValue)
{
	auto it = FloatData.find(key);
	if (it != FloatData.end())
	{
		outValue = FloatData[key];
		return true;
	}
	return false;
}

void Blackboard::SetBool(const string& key, bool value)
{
	BoolData[key] = value;
}

bool Blackboard::GetBool(const string& key, bool& outValue)
{
	auto it = BoolData.find(key);
	if (it != BoolData.end())
	{
		outValue = BoolData[key];
		return true;
	}
	return false;
}

void Blackboard::SetActor(const string& key, Actor* value)
{
	ActorData[key] = value;
}

bool Blackboard::GetActor(const string& key, Actor*& outValue)
{
	auto it = ActorData.find(key);
	if (it != ActorData.end())
	{
		outValue = ActorData[key];
		return true;
	}
	return false;
}
