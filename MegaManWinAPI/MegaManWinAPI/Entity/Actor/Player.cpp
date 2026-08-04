#include "pch.h"
#include "Player.h"
#include "AnimatorComponent.h"
#include "BoxCollider.h"
#include "InputManager.h" // 입력을 받기 위해 포함
#include "TimeManager.h"  // DeltaTime을 사용하기 위해 포함
#include "RigidBodyComponent.h"
#include "FSMComponent.h"
#include "WeaponComponent.h"
// 테스트를 위한 include
#include "ImageRenderer.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "IdleState.h"
#include "MoveState.h"
#include "JumpState.h"
void Player::Init()
{
	Super::Init();

	// 1. 애니메이터 컴포넌트 추가
	AnimatorComponent* animator = AddComponent<AnimatorComponent>();
	animator->LoadAnimationFromJson(L"Idle", L"../Resources/sprites/Player/Animation/idle.json");
	animator->Play(L"Idle");

	// 테스트용 이미지 렌더러 컴포넌트 추가
	/*ImageRenderer* render = AddComponent<ImageRenderer>();
	Texture * tex = ResourceManager::GetInstance().GetTexture(L"Player");
	render->SetTexture(tex);*/

	// 충돌체(Collider) 컴포넌트 추가
	BoxCollider* collider = AddComponent<BoxCollider>();
	collider->SetSize(50.0f, 50.0f);

	// 중력(Rigidbody) 컴포넌트 추가
	RigidBodyComponent* rigidbody = AddComponent<RigidBodyComponent>();

	// FSM 및 Weapon 컴포넌트 부착
	FSMComponent* fsm = AddComponent<FSMComponent>();
	
	fsm->AddState("Idle", new IdleState(fsm));
	fsm->AddState("Move", new MoveState(fsm));
	fsm->AddState("Jump", new JumpState(fsm));
	fsm->ChangeState("Idle");

	WeaponComponent* weapon = AddComponent<WeaponComponent>();
	weapon->Init();
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime); // 부모 업데이트 호출 (여기서 부착된 컴포넌트들의 Update가 자동 실행됩니다)
}

void Player::OnStay(Actor* other, const HitResult& hit)
{
	Vector pos = GetPos();
	bool isWall = (other->GetActorType() == ActorType::WALL);
	bool isGround = (other->GetActorType() == ActorType::Ground);

	BoxCollider* myCol = GetComponent<BoxCollider>();
	BoxCollider* otherCol = other->GetComponent<BoxCollider>();
	if (!myCol || !otherCol) return;

	Vector myColPos = myCol->GetColliderPos();
	Vector otherColPos = otherCol->GetColliderPos();

	if (isWall)
	{
		float mySize = myCol->GetWidth() / 2.0f;
		float otherSize = otherCol->GetWidth() / 2.0f;

		float distanceX = abs(myColPos.x - otherColPos.x);
		float overlapX = (mySize + otherSize) - distanceX;

		if (overlapX > 0.0f)
		{
			if (myColPos.x < otherColPos.x)
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
	if (isGround)
	{
		float mySize = myCol->GetHeight() / 2.0f;
		float otherSize = otherCol->GetHeight() / 2.0f;

		float distanceY = abs(myColPos.y - otherColPos.y);
		float overlapY = (mySize + otherSize) - distanceY;

		if (overlapY > 0.0f)
		{
			// 부동소수점 오차로 인한 충돌 해제를 막기 위해 미세하게 덜 밀어냄
			float pushOut = overlapY - 0.1f;
			
			// pushOut 값이 아주 미세한 오차 범위(0.001f) 이하라면 위치를 이동하지 않음 (픽셀 덜덜거림 방지)
			if (std::abs(pushOut) > 0.001f)
			{
				pos.y -= pushOut;
			}
		}

		RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
		
		// 플레이어가 아래로 떨어지고 있거나 가만히 있을 때만 바닥 착지 처리
		// (점프해서 위로 올라가고 있을 때는 속도를 0으로 깎지 않음!)
		if (rigid->GetVelocity().y >= 0.0f)
		{
			rigid->SetGrounded(true);
			rigid->SetVelocity({ 0.0f, 0.0f });
		}
	}
	SetPos(pos);
	
}

void Player::OnExit(Actor* other)
{
	RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
	if (other->GetActorType() == ActorType::Ground)
	{
		rigid->SetGrounded(false);
	}
}
