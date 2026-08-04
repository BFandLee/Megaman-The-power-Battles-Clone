#include "pch.h"
#include "JumpState.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Player.h"
#include "InputManager.h"
#include "RigidBodyComponent.h"

JumpState::~JumpState()
{
}

void JumpState::Enter()
{
	_rigidbody = m_pOwnerFSM->GetOwner()->GetComponent<RigidBodyComponent>();
	float jumpForce = m_pOwnerFSM->GetJumpForce();
	_rigidbody->SetVelocity({ 0.0f, -jumpForce });
	_rigidbody->SetGrounded(false);

	AnimatorComponent* pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (pAnimator != nullptr)
	{
		pAnimator->Play(L"Jump");
	}
}

void JumpState::Update(float deltaTime)
{
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

	m_pOwnerFSM->GetOwner()->SetPos(pos);

	if (_rigidbody->IsGrounded())
	{
		m_pOwnerFSM->ChangeState("Idle");
	}
}

void JumpState::Exit()
{
}
