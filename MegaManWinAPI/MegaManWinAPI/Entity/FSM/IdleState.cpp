#include "pch.h"
#include "IdleState.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Player.h"
#include "InputManager.h"
#include "WeaponComponent.h"

IdleState::IdleState(FSMComponent* pOwner)
    : State(pOwner)
{
}

IdleState::~IdleState()
{
}

void IdleState::Enter()
{
    _pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (_pAnimator != nullptr)
	{
		_pAnimator->Play(L"Idle");
	}
	
}

void IdleState::Update(float deltaTime)
{
	bool isAttacking = m_pOwnerFSM->GetOwner()->GetComponent<WeaponComponent>()->IsAttacking();
	if (isAttacking)
	{
		_pAnimator->Play(L"IdleAttack");
	}
	else
	{
		_pAnimator->Play(L"Idle");
	}

	
	if (InputManager::GetInstance().GetButtonPressed(KeyType::Right) 
		|| InputManager::GetInstance().GetButtonPressed(KeyType::Left))
	{
		m_pOwnerFSM->ChangeState("Move");
	}

	if (InputManager::GetInstance().GetButtonPressed(KeyType::SpaceBar))
	{
		m_pOwnerFSM->ChangeState("Jump");
	}
	
}

void IdleState::Exit()
{
}
