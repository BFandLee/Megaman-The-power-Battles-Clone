#include "pch.h"
#include "MoveState.h"
#include "AnimatorComponent.h"
#include "FSMComponent.h"
#include "Player.h"
#include "InputManager.h"
#include "WeaponComponent.h"
#include <RigidBodyComponent.h>

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
		_pAnimator->Play(L"Move");
	}
}

void MoveState::Update(float deltaTime)
{
	// 애니메이션 변경
	bool isAttacking = m_pOwnerFSM->GetOwner()->GetComponent<WeaponComponent>()->IsAttacking();
	if (isAttacking)
	{
		_pAnimator->Play(L"MoveAttack", true);
	}
	else
	{
		_pAnimator->Play(L"Move", true);
	}

    RigidBodyComponent* rigid = m_pOwnerFSM->GetOwner()->GetComponent<RigidBodyComponent>();
    Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());

    // 가속도 및 최대 속도 값 
    float accel = m_pOwnerFSM->GetAccel();
    float maxSpeed = m_pOwnerFSM->GetMoveSpeed();
    if (InputManager::GetInstance().GetButtonPressed(KeyType::Right))
    {
        player->SetLookDirX(1.0f);
        rigid->AddVelocity(Vector(accel * deltaTime, 0));
    }
    else if (InputManager::GetInstance().GetButtonPressed(KeyType::Left))
    {
        player->SetLookDirX(-1.0f);
        rigid->AddVelocity(Vector(-accel * deltaTime, 0));
    }
    else
    {
        // 키를 뗐을 때 즉시 멈추고 싶다면 _velocity.x = 0; 으로 두거나,
        // 마찰력으로 자연스럽게 멈추게 두고 State만 Idle로 전환할 수도 있습니다.
        m_pOwnerFSM->ChangeState("Idle");
        rigid->SetVelocity(Vector(0.0f, rigid->GetVelocity().y));
    }

    // 2. 최대 속도 제한 (Clamp)
    Vector v = rigid->GetVelocity();
    v.x = std::clamp(v.x, -maxSpeed, maxSpeed);
    rigid->SetVelocity(v);


	if (InputManager::GetInstance().GetButtonPressed(KeyType::SpaceBar))
	{
		m_pOwnerFSM->ChangeState("Jump");
	}
}

void MoveState::Exit()
{
}
