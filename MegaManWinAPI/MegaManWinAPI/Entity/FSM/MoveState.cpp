#include "pch.h"
#include "MoveState.h"
#include "AnimatorComponent.h"
#include "FSMComponent.h"
#include "Player.h"
#include "InputManager.h"
#include "WeaponComponent.h"

MoveState::MoveState(FSMComponent* pOwner) : State(pOwner)
{
}

MoveState::~MoveState()
{
}

void MoveState::Enter()
{
	_pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (_pAnimator != nullptr)
	{
		_pAnimator->Play(L"Run");
	}
}

void MoveState::Update(float deltaTime)
{
	// 애니메이션 변경
	bool isAttacking = m_pOwnerFSM->GetOwner()->GetComponent<WeaponComponent>()->IsAttacking();
	if (isAttacking)
	{
		_pAnimator->Play(L"MoveAttack");
	}
	else
	{
		_pAnimator->Play(L"Run");
	}

	Vector pos = m_pOwnerFSM->GetOwner()->GetPos();
	Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());
	if (InputManager::GetInstance().GetButtonPressed(KeyType::Right))
	{
		pos.x += m_pOwnerFSM->GetMoveSpeed() * deltaTime;
		player->SetLookDirX(1.0f);
	}
	else if (InputManager::GetInstance().GetButtonPressed(KeyType::Left))
	{
		pos.x -= m_pOwnerFSM->GetMoveSpeed() * deltaTime;
		player->SetLookDirX(-1.0f);
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
