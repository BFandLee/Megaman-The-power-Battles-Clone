#include "pch.h"
#include "SpawnState.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Player.h"
#include "RigidBodyComponent.h"

void SpawnState::Enter()
{
    Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());

    _pAnimator = player->GetComponent<AnimatorComponent>();
    RigidBodyComponent* rigid = player->GetComponent<RigidBodyComponent>();

    _pAnimator->Play(L"SponeDrop");

    player->SetInvincible(true);
    _isLanded = false;

    
   

    rigid->SetGravity(0.0f);
}

void SpawnState::Update(float deltaTime)
{
    Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());
    RigidBodyComponent* rigid = player->GetComponent<RigidBodyComponent>();
    
    if (_isFirstFrame)
    {
        Vector currentPos = player->GetPos();
        currentPos.x = 65.0f;
        currentPos.y -= 465.0f;
        player->SetPos(currentPos);
        rigid->SetGravity(10000.0f);
        _isFirstFrame = false;

        return;
    }

    if (rigid->IsGrounded() && !_isLanded)
    {
        rigid->SetGravity(980.0f);
        _isLanded = true;

        _pAnimator->Play(L"Spone");

        _pAnimator->SetEndEvent(L"Spone", [this]() {
            m_pOwnerFSM->ChangeState("Idle");
            });
    }

}

void SpawnState::Exit()
{
    Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());

    // TODO: (4) 스폰이 끝났으므로 플레이어의 무적 상태를 다시 해제하세요.
    player->SetInvincible(false);
    // (선택) 무적이 바로 풀리는게 싫다면, _invincibleTimer를 조작하여 
    // 스폰 직후에도 잠깐 무적이 유지되게 할 수도 있습니다.
}