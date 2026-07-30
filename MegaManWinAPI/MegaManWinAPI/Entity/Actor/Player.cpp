#include "pch.h"
#include "Player.h"
#include "AnimatorComponent.h"

void Player::Init()
{
	AnimatorComponent* animator = AddComponent<AnimatorComponent>();

	// animator->LoadAnimationFromJson(L"Idle", L"Resources/sprites/idle.json");

	animator->Play(L"Idle");
}