#include "pch.h"
#include "SpawnState.h"
#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "Player.h"
#include "RigidBodyComponent.h"
#include "WeaponComponent.h"

void SpawnState::Enter()
{
    Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());

    if (player != nullptr)
    {
        WeaponComponent* weapon = player->GetComponent<WeaponComponent>();
        if (weapon != nullptr)
        {
            weapon->CancelCharge();
        }
    }

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
    }

}

void SpawnState::Exit()
{
    Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());

    // 스폰이 끝났으므로 플레이어의 무적 상태를 다시 해제
    if (player != nullptr)
    {
        player->SetInvincible(false);
    }
}
