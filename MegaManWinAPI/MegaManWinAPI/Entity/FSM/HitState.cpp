#include "pch.h"
#include "HitState.h"
#include "FSMComponent.h"
#include "Actor.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "Player.h"
#include "InputManager.h"

HitState::HitState(FSMComponent* fsm) : State(fsm)
{
}

HitState::~HitState()
{
}

void HitState::Enter()
{
	_hitTimer = 0.0f;
	

	// TODO 1: 피격 애니메이션 재생 (예: "Hit")
	_pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (_pAnimator != nullptr)
	{
		_pAnimator->Play(L"Hit");
	}

	// TODO 2: FSM 컴포넌트의 Owner(Player)를 가져와서 RigidBodyComponent를 얻고 뒤로 밀려나는 넉백(Velocity 설정)을 구현하세요.
	Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());
	RigidBodyComponent* rigid = player->GetComponent<RigidBodyComponent>();
	float HitX = player->GetHitDirX() * KNOCKBACK;
	rigid->SetVelocity(Vector(HitX, RESTORING));
}

void HitState::Update(float deltaTime)
{
	_hitTimer += deltaTime;

	// TODO 3: _hitTimer가 일정 시간(예: 0.5초) 경과하면 다시 "Idle" 상태로 돌아가도록 구현하세요.

	if (_hitTimer >= _maxhitTimer)
	{
		// 플레이어가 좌/우 이동 키를 누르고 있다면 바로 Move 상태로!
		if (InputManager::GetInstance().GetButtonPressed(KeyType::Left) ||
			InputManager::GetInstance().GetButtonPressed(KeyType::Right))
		{
			m_pOwnerFSM->ChangeState("Move");
		}
		else
		{
			// 키를 누르고 있지 않다면 자연스럽게 대기 상태로!
			m_pOwnerFSM->ChangeState("Idle");
		}
	}

}

void HitState::Exit()
{
}
