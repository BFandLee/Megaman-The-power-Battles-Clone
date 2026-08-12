#include "pch.h"
#include "BossSpawn.h"
#include "FSMComponent.h"

BossSpawn::BossSpawn(FSMComponent* fsm) : State(fsm)
{
}

BossSpawn::~BossSpawn()
{
}

void BossSpawn::Enter()
{
}

void BossSpawn::Update(float deltaTime)
{
	m_pOwnerFSM->ChangeState("Phase1");
}

void BossSpawn::Exit()
{
}
