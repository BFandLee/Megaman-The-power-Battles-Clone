#include "pch.h"
#include "BossSpawn.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Actor.h"

BossSpawn::BossSpawn(FSMComponent* fsm) : State(fsm)
{
}

BossSpawn::~BossSpawn()
{
}

void BossSpawn::Enter()
{
	_pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	_pAnimator->Play(L"Spawn", true);
}

void BossSpawn::Update(float deltaTime)
{
	m_pOwnerFSM->ChangeState("Phase1");
}

void BossSpawn::Exit()
{
}
