#include "pch.h"
#include "MoveState.h"
#include "AnimatorComponent.h"
#include "FSMComponent.h"
#include "Player.h"
#include "InputManager.h"

MoveState::MoveState(FSMComponent* pOwner) : State(pOwner)
{
}

MoveState::~MoveState()
{
}

void MoveState::Enter()
{
	AnimatorComponent* pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (pAnimator != nullptr)
	{
		pAnimator->Play(L"Run");
	}
}

void MoveState::Update(float deltaTime)
{
	Vector pos = m_pOwnerFSM->GetOwner()->GetPos();
	if (InputManager::GetInstance().GetButtonPressed(KeyType::Right))
	{
		pos.x += m_pOwnerFSM->GetMoveSpeed() * deltaTime;
	}
	else if (InputManager::GetInstance().GetButtonPressed(KeyType::Left))
	{
		pos.x -= m_pOwnerFSM->GetMoveSpeed() * deltaTime;
	}
	else
	{
		m_pOwnerFSM->ChangeState("Idle");
	}

	m_pOwnerFSM->GetOwner()->SetPos(pos);

	if (InputManager::GetInstance().GetButtonPressed(KeyType::SpaceBar))
	{
		m_pOwnerFSM->ChangeState("Jump");
	}
}

void MoveState::Exit()
{
}
