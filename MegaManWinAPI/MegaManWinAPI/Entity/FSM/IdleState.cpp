#include "pch.h"
#include "IdleState.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Player.h" // Player 객체 접근용 (필요 시)

IdleState::IdleState(FSMComponent* pOwner)
    : State(pOwner)
{
}

IdleState::~IdleState()
{
}

void IdleState::Enter()
{
    // TODO: FSM의 주인(Player)의 AnimatorComponent를 가져와서 대기(Idle) 애니메이션 재생 지시
}

void IdleState::Update(float deltaTime)
{
    // TODO: 키보드 입력을 검사하고, 좌/우 방향키가 눌렸다면 m_pOwnerFSM->ChangeState("Move") 호출
}

void IdleState::Exit()
{
}
