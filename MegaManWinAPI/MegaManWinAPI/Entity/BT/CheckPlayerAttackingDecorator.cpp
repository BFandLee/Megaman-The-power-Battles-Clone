#include "pch.h"
#include "CheckPlayerAttackingDecorator.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Actor.h"

bool CheckPlayerAttackingDecorator::checkCondition(Blackboard* bb)
{
	Scene* currentScene = SceneManager::GetInstance().GetScene();
	if (currentScene == nullptr)
	{
		return false;
	}
	Actor* bullet = currentScene->FindActorByType(ActorType::PlayerBullet);
	if (bullet == nullptr)
	{
		return false;
	}

	bool isAttacking = (bullet != nullptr);
	return isAttacking == _checkAttacking;
}

void CheckPlayerAttackingDecorator::DrawProperty()
{
	ImGui::Checkbox("Check Attacking", &_checkAttacking);
}
