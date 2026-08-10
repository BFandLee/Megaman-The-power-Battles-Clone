#include "pch.h"
#include "Player.h"
#include "AnimatorComponent.h"
#include "BoxCollider.h"
#include "InputManager.h" // 입력을 받기 위해 포함
#include "TimeManager.h"  // DeltaTime을 사용하기 위해 포함
#include "RigidBodyComponent.h"
#include "FSMComponent.h"
#include "WeaponComponent.h"
#include "TransformComponent.h"
// 테스트를 위한 include
#include "Texture.h"
#include "ResourceManager.h"
#include "IdleState.h"
#include "MoveState.h"
#include "JumpState.h"
#include "SlideState.h"
#include "HitState.h"
#include "DeathState.h"
#include "SpawnState.h"

void Player::Init()
{
	Super::Init();

	// 1. 애니메이터 컴포넌트 추가
	AnimatorComponent* animator = AddComponent<AnimatorComponent>();
	animator->LoadAnimationFromJson(L"Idle", L"../Resources/sprites/Player/Animation/idle.json");
	animator->LoadAnimationFromJson(L"Move", L"../Resources/sprites/Player/Animation/Move.json");
	animator->LoadAnimationFromJson(L"Jump", L"../Resources/sprites/Player/Animation/Jump.json");
	animator->LoadAnimationFromJson(L"Sliding", L"../Resources/sprites/Player/Animation/Sliding.json");
	animator->LoadAnimationFromJson(L"Hit", L"../Resources/sprites/Player/Animation/Hit.json");
	animator->LoadAnimationFromJson(L"Death", L"../Resources/sprites/Player/Animation/Death.json");
	animator->LoadAnimationFromJson(L"Spone", L"../Resources/sprites/Player/Animation/Spone.json");
	animator->LoadAnimationFromJson(L"SponeDrop", L"../Resources/sprites/Player/Animation/SponeDrop.json");
	// animator->Play(L"Idle");

	// 충돌체(Collider) 컴포넌트 추가
	BoxCollider* collider = AddComponent<BoxCollider>();
	collider->SetSize(50.0f, 50.0f);

	// 중력(Rigidbody) 컴포넌트 추가
	RigidBodyComponent* rigidbody = AddComponent<RigidBodyComponent>();

	// FSM 및 Weapon 컴포넌트 부착
	FSMComponent* fsm = AddComponent<FSMComponent>();
	
	//StateMachine으로 정리하자
	fsm->AddState("Idle", new IdleState(fsm));
	fsm->AddState("Move", new MoveState(fsm));
	fsm->AddState("Jump", new JumpState(fsm));
	fsm->AddState("Sliding", new SlideState(fsm));
	fsm->AddState("Hit", new HitState(fsm));
	fsm->AddState("Death", new DeathState(fsm));
	fsm->AddState("Spawn", new SpawnState(fsm));
	fsm->ChangeState("Spawn");

	WeaponComponent* weapon = AddComponent<WeaponComponent>();
	weapon->Init();

	// 스왑용 이미지 생성
	std::unordered_map<uint32, uint32> yellowPalette = {
	{ 0xFF5084F4, 0xFFF0C030 }, // 메인 파란색 -> 메인 노란색
	{ 0xFF2040D4, 0xFFF08000 }, // 어두운 파란색 -> 어두운 노란색
	{ 0xFF30C4D4, 0xFFF0F0C0 }  // 밝은 파란색 -> 밝은 노란색
};
	// Idle
	Texture* idleYellow = new Texture();
	idleYellow->LoadWithPaletteSwap(L"../Resources/sprites/Player/State/Player_Idle_Sheet.png", yellowPalette, 1, 1, 1.0f, false);
	animator->SetSwapTextureForState(L"Idle", idleYellow);

	// Move
	Texture* moveYellow = new Texture();
	moveYellow->LoadWithPaletteSwap(L"../Resources/sprites/Player/State/Player_Walk_Sheet.png", yellowPalette, 1, 1, 1.0f, false);
	animator->SetSwapTextureForState(L"Move", moveYellow);

	// Jump
	Texture* jumpYellow = new Texture();
	jumpYellow->LoadWithPaletteSwap(L"../Resources/sprites/Player/State/Player_Jump_Sheet.png", yellowPalette, 1, 1, 1.0f, false);
	animator->SetSwapTextureForState(L"Jump", jumpYellow);

	// Sliding
	Texture* slideYellow = new Texture();
	slideYellow->LoadWithPaletteSwap(L"../Resources/sprites/Player/State/Player_Sliding_Sheet.png", yellowPalette, 1, 1, 1.0f, false);
	animator->SetSwapTextureForState(L"Sliding", slideYellow);
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime); 

	AnimatorComponent* animator = GetComponent<AnimatorComponent>();
	if (InputManager::GetInstance().GetButtonPressed(KeyType::A))
	{
		_chargeTime += deltaTime;

		if (_chargeTime > MAX_CHARGE_TIME)
		{
			_blinkTime += deltaTime;

			if (_blinkTime > 0.5f)
			{
				_blinkTime = 0.0f;
				_isYellowColor = !_isYellowColor;
				animator->SetTextureColor(_isYellowColor);
			}
		}

	}
	else if (InputManager::GetInstance().GetButtonUp(KeyType::A))
	{
		_chargeTime = 0.0f;
		_blinkTime = 0.0f;
		animator->SetTextureColor(false);
	}
	

	if (_isInvincible)
	{
		_invincibleTimer += deltaTime;

		if (_invincibleTimer > 1.5f)
		{
			_isInvincible = false;
		}
	}
	
}

void Player::Render(ID2D1RenderTarget* renderTarget)
{
	// 무적 상태일 때만 특정 조건에서 return 시켜서 렌더링을 건너뜀 (예: 0.1초 단위로 깜빡임)
	if (_isInvincible)
	{
		if ((int32)(_invincibleTimer * 10) % 2)
			return;
	}

	Super::Render(renderTarget);
}

void Player::OnStay(Actor* other, const HitResult& hit)
{
	Vector pos = GetPos();
	bool isWall = (other->GetActorType() == ActorType::WALL);
	bool isGround = (other->GetActorType() == ActorType::Ground);
	bool isEnemy = (other->GetActorType() == ActorType::Enemy);

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
				pos.x -= overlapX;
			}
			else
			{
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
			float pushOut = std::max(overlapY - 0.1f , 0.0f);
			
			// pushOut 값이 아주 미세한 오차 범위(0.001f) 이하라면 위치를 이동하지 않음 (픽셀 덜덜거림 방지)
			if (std::abs(pushOut) > 0.001f)
			{
				pos.y -= pushOut;
			}
		}

		RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
		
		if (rigid->GetVelocity().y >= 0.0f)
		{
			rigid->SetGrounded(true);
			Vector currentVel = rigid->GetVelocity();
			currentVel.y = 0.0f; // Y축(떨어지는 속도)만 0으로 초기화

			rigid->SetVelocity(currentVel);
		}
	}

	if (isEnemy && !_isInvincible)
	{
		// TODO 3: 플레이어와 적의 x좌표를 비교하여 넉백 방향(hitDirX)을 계산하고 TakeDamage를 호출하세요. (예: 적이 오른쪽에 있으면 -1.0f)
		float diffX = this->GetPos().x - other->GetPos().x;

		// 방향벡터만 남기기 위해서 정규화
		float hitDirX = 0.0f;
		if (diffX > 0)
		{
			hitDirX = 1.0f;
		}
		else if(diffX < 0)
		{
			hitDirX = -1.0f;
		}

		TakeDamage(5, hitDirX);
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

void Player::SetLookDirX(float dir)
{
	_lookdirX = dir;

	// 2. TransformComponent 가져오기
	TransformComponent* transform = GetComponent<TransformComponent>();

	if (transform)
	{
		Vector currentScale = transform->GetScale();
		currentScale.x = abs(currentScale.x) * dir;
		transform->SetScale(currentScale);
	}
}

void Player::TakeDamage(int damage, float hitDirX)
{
	if (_hp > 0)
	{
		_hp -= damage;
		_isInvincible = true;
		_invincibleTimer = 0.0f;
		this->GetComponent<FSMComponent>()->ChangeState("Hit");
	}

	if (_hp < 0)
	{
		this->GetComponent<FSMComponent>()->ChangeState("Death");
	}
	_hitdirX = hitDirX;
}
