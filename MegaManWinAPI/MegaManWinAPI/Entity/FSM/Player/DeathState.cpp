#include "pch.h"
#include "DeathState.h"
#include "FSMComponent.h"
#include "Actor.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "Player.h"
#include "BoxCollider.h"
#include "SoundManager.h"

DeathState::DeathState(FSMComponent* fsm) : State(fsm)
{
}

DeathState::~DeathState()
{
}

void DeathState::Enter()
{
	_deathTimer = 0.0f;

	SoundManager::GetInstance().StopSFX(L"megaman_charge");
	SoundManager::GetInstance().StopSFX(L"megaman_chargeend");
	SoundManager::GetInstance().PlaySFX(L"megaman_death");

	_pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
	if (_pAnimator != nullptr)
	{
		_pAnimator->Play(L"Dead");
	}
	
	// TODO 2: 이동 속도를 0으로 만들어 더 이상 움직이지 않도록 처리 (RigidBodyComponent 활용)
	Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());
	RigidBodyComponent* rigid = player->GetComponent<RigidBodyComponent>();
	rigid->SetVelocity(Vector(0.0f, 0.0f));
	
	// TODO 3: 콜라이더를 비활성화 하거나 크기를 0으로 만들어 더 이상 충돌하지 않게 처리
	BoxCollider* collider = player->GetComponent<BoxCollider>();
	collider->SetSize(0.0f, 0.0f);
}

void DeathState::Update(float deltaTime)
{
	_deathTimer += deltaTime;

	// TODO 4: 데스 애니메이션이 끝날 즈음(예: 1.5초 등) 일정 시간이 지나면 액터를 삭제 상태(PendingKill)로 만들기
	if (_deathTimer > 1.5f)
	{
		m_pOwnerFSM->GetOwner()->Destroy();
	}

}

void DeathState::Exit()
{
}
