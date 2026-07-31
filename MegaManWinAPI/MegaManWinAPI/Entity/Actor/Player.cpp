#include "pch.h"
#include "Player.h"
#include "AnimatorComponent.h"
#include "BoxCollider.h"
#include "InputManager.h" // 입력을 받기 위해 포함
#include "TimeManager.h"  // DeltaTime을 사용하기 위해 포함

// 테스트를 위한 include
#include "ImageRenderer.h"
#include "Texture.h"
#include "ResourceManager.h"

void Player::Init()
{
	Super::Init();
	_state = PlayerState::Idle;

	// 1. 애니메이터 컴포넌트 추가
	//AnimatorComponent* animator = AddComponent<AnimatorComponent>();
	//// animator->LoadAnimationFromJson(L"Idle", L"Resources/sprites/idle.json");
	//animator->Play(L"Idle");

	// 테스트용 이미지 렌더러 컴포넌트 추가
	ImageRenderer* render = AddComponent<ImageRenderer>();
	Texture * tex = ResourceManager::GetInstance().GetTexture(L"Player");
	render->SetTexture(tex);

	// 2. 충돌체(Collider) 컴포넌트 추가
	BoxCollider* collider = AddComponent<BoxCollider>();
	collider->SetSize(50.0f, 50.0f); 
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime); // 부모 업데이트 호출

	switch (_state)
	{
		case PlayerState::Idle:
		{
			break;
		}
		case PlayerState::Run:
		{
			break;
		}
	}
	
	// TODO 5: InputManager를 사용하여 좌/우 방향키 입력 시 m_Pos.x 값을 변경하고,
	// 이동 중일 때는 m_State를 Run으로, 멈췄을 때는 Idle로 변경해보세요.
	Vector pos = GetPos();
	if (InputManager::GetInstance().GetButtonPressed(KeyType::Right))
	{
		_state = PlayerState::Run;
		pos.x += _speed * TimeManager::GetInstance().GetDT();
	}
	else if (InputManager::GetInstance().GetButtonPressed(KeyType::Left))
	{
		_state = PlayerState::Run;
		pos.x -= _speed * TimeManager::GetInstance().GetDT();
	}
	else
	{
		_state = PlayerState::Idle;
	}

	SetPos(pos);

	
}

void Player::OnStay(Actor* other, const HitResult& hit)
{
	Vector pos = GetPos();
	bool isWall = (other->GetActorType() == ActorType::WALL);

	// X축 겹친 깊이 : (actor의 width/2 + other.width/2) - (actor.pos-other.pos)
	if (isWall)
	{
		float mySize = GetComponent<BoxCollider>()->GetWidth() / 2.0f;
		float otherSize = other->GetComponent<BoxCollider>()->GetWidth() / 2.0f;

		float distanceX = abs(pos.x - other->GetPos().x);

		float overlapX = (mySize + otherSize) - distanceX;

		if (overlapX > 0.0f)
		{
			if (pos.x < other->GetPos().x)
			{
				// 왼쪽으로 밀기
				pos.x -= overlapX;
			}
			else
			{
				// 오른쪽으로 밀기
				pos.x += overlapX;
			}
		}
	}
	SetPos(pos);
	
}