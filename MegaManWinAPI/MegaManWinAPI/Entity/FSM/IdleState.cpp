#include "pch.h"
#include "IdleState.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Player.h"
#include "InputManager.h"

IdleState::IdleState(FSMComponent* pOwner)
    : State(pOwner)
{
}

IdleState::~IdleState()
{
}

void IdleState::Enter()
{
    AnimatorComponent* pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (pAnimator != nullptr)
	{
		pAnimator->Play(L"Idle");
	}
	
}

void IdleState::Update(float deltaTime)
{
    // TODO: 키보드 입력을 검사하고, 좌/우 방향키가 눌렸다면 m_pOwnerFSM->ChangeState("Move") 호출
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
