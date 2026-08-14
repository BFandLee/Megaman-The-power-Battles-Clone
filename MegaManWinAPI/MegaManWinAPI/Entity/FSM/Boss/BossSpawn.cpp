#include "pch.h"
#include "BossSpawn.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Actor.h"
#include "Boss.h"
#include "RigidBodyComponent.h"

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
    _isLanded = false;
}

void BossSpawn::Update(float deltaTime)
{
    Boss* boss = static_cast<Boss*>(m_pOwnerFSM->GetOwner());
    RigidBodyComponent* rigid = boss->GetComponent<RigidBodyComponent>();

    if (_isFirstFrame)
    {
        Vector currentPos = boss->GetPos();
        currentPos.x = 1000.0f;
        currentPos.y -= 465.0f;
        boss->SetPos(currentPos);
        rigid->SetGravity(10000.0f);
        _isFirstFrame = false;

        return;
    }

    if (rigid->IsGrounded() && !_isLanded)
    {
        rigid->SetGravity(980.0f);
        _isLanded = true;

        _pAnimator->Play(L"Spawn");

        _pAnimator->SetEndEvent(L"Spawn", [this]() {
            m_pOwnerFSM->ChangeState("Phase1");
            });
    }
}

void BossSpawn::Exit()
{
}
