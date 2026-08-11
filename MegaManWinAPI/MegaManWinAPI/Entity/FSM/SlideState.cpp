#include "pch.h"
#include "SlideState.h"
#include "AnimatorComponent.h"
#include "FSMComponent.h"
#include "Player.h"
#include "InputManager.h"
#include "RigidBodyComponent.h"
#include "BoxCollider.h"
#include "MovementComponent.h"

SlideState::SlideState(FSMComponent* pOwner) : State(pOwner)
{
    _slideVelocity = 0.0f;
    _friction = 0.0f;
    _originalHeight = 0.0f;
    _originalOffsetY = 0.0f;
}

SlideState::~SlideState()
{
}

void SlideState::Enter()
{
    _pAnimator = m_pOwnerFSM->GetOwner()->GetComponent<AnimatorComponent>();
    
    _pAnimator->Play(L"Sliding");
    Player* player = static_cast<Player*>(m_pOwnerFSM->GetOwner());
    BoxCollider* collider = player->GetComponent<BoxCollider>();
    MovementComponent* movement = m_pOwnerFSM->GetOwner()->GetComponent<MovementComponent>();

    if (collider != nullptr)
    {
        _originalHeight = collider->GetHeight();
        _originalOffsetY = collider->GetOffset().y;
        
        float currentHeight = collider->GetHeight() / 2.0f;
        float currentOffsetY = _originalOffsetY + (_originalHeight / 4.0f);
        collider->SetSize(collider->GetWidth(), currentHeight);
        collider->SetOffset(Vector(collider->GetOffset().x, currentOffsetY));

    }
    float dir = player->GetLookDirX();
    if (movement != nullptr)
    {
        _slideVelocity = dir * movement->GetSlideVelcoity();
        _friction = movement->GetFriction();
    }
   
}

void SlideState::Update(float deltaTime)
{
    if (_slideVelocity > 0.0f)
    {
        _slideVelocity = std::max(_slideVelocity - (_friction * deltaTime), 0.0f); // 오른쪽 이동 → 감속
    }
    else if (_slideVelocity < 0.0f)
    {
        _slideVelocity = std::min(_slideVelocity + (_friction * deltaTime), 0.0f); // 왼쪽 이동 → 감속
    }

    RigidBodyComponent* rigid = m_pOwnerFSM->GetOwner()->GetComponent<RigidBodyComponent>();
    rigid->SetVelocity(Vector(_slideVelocity, rigid->GetVelocity().y));

    if (_slideVelocity == 0.0f)
    {
        m_pOwnerFSM->ChangeState("Idle");
    }

    if (InputManager::GetInstance().GetButtonPressed(KeyType::SpaceBar))
    {
        m_pOwnerFSM->ChangeState("Jump");
    }
}

void SlideState::Exit()
{
    // Enter에서 백업해 둔 _originalHeight, _originalOffsetY를 사용해 BoxCollider의 크기와 오프셋을 원래대로 원복하기
    BoxCollider* collider = m_pOwnerFSM->GetOwner()->GetComponent<BoxCollider>();
    collider->SetSize(collider->GetWidth(), _originalHeight);
    collider->SetOffset(Vector(collider->GetOffset().x, _originalOffsetY));
}
