#include "pch.h"
#include "JumpState.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Player.h"
#include "InputManager.h"
#include "RigidBodyComponent.h"
#include "WeaponComponent.h"
#include "MovementComponent.h"

JumpState::~JumpState()
{
}

void JumpState::Enter()
{
	_rigidbody = m_pOwnerFSM->GetOwner()->GetComponent<RigidBodyComponent>();
	_movement = m_pOwnerFSM->GetOwner()->GetComponent<MovementComponent>();

	float jumpForce = _movement->GetJumpForce();
	_rigidbody->SetVelocity({ 0.0f, -jumpForce });
	_rigidbody->SetGrounded(false);

	_pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (_pAnimator != nullptr)
	{
		_pAnimator->Play(L"Jump");
	}
}

void JumpState::Update(float deltaTime)
{
	bool isAttacking = m_pOwnerFSM->GetOwner()->GetComponent<WeaponComponent>()->IsAttacking();
	if (isAttacking)
	{
		_pAnimator->Play(L"JumpAttack", true);
	}
	else
	{
		_pAnimator->Play(L"Jump", true);
	}


	Vector pos = m_pOwnerFSM->GetOwner()->GetPos();
	Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());
	if (InputManager::GetInstance().GetButtonPressed(KeyType::Right))
	{
		pos.x += _movement->GetMoveSpeed() * deltaTime;
		player->SetLookDirX(1.0f);
	}
	else if (InputManager::GetInstance().GetButtonPressed(KeyType::Left))
	{
		pos.x -= _movement->GetMoveSpeed() * deltaTime;
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
