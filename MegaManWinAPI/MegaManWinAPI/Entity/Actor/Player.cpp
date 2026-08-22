#include "pch.h"
#include "Player.h"
#include "AnimatorComponent.h"
#include "BoxCollider.h"
#include "InputManager.h" 
#include "TimeManager.h"  
#include "RigidBodyComponent.h"
#include "FSMComponent.h"
#include "WeaponComponent.h"
#include "TransformComponent.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "MovementComponent.h"


void Player::Init()
{
	// Idempotent 패턴
	if (_isInit) return;
	_isInit = true;
	Super::Init();
	
	AddComponent<AnimatorComponent>();
	// 충돌체(Collider) 컴포넌트 추가
	BoxCollider* collider = AddComponent<BoxCollider>();
	collider->SetSize(50.0f, 50.0f);

	// 중력(Rigidbody) 컴포넌트 추가
	RigidBodyComponent* rigidbody = AddComponent<RigidBodyComponent>();

	FSMComponent* fsm = AddComponent<FSMComponent>();
	
	WeaponComponent* weapon = AddComponent<WeaponComponent>();
	weapon->Init();

	MovementComponent* movement = AddComponent<MovementComponent>();
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
				if (animator != nullptr)
					animator->SetTextureColor(_isYellowColor);
			}
		}
	}
	else
	{
		_chargeTime = 0.0f;
		_blinkTime = 0.0f;
		if (animator != nullptr)
			animator->SetTextureColor(false);
	}

	if (_isInvincible)
	{
		_invincibleTimer += deltaTime;

		if (_invincibleTimer > 2.0f)
		{
			_isInvincible = false;
		}
	}
}

void Player::Render(ID2D1RenderTarget* renderTarget)
{
	// 무적 상태일 때만 특정 조건에서 return 시켜서 렌더링을 건너뜀 (0.2초 단위로 깜빡임)
	if (_isInvincible)
	{
		if ((int32)(_invincibleTimer * 5) % 2)
			return;
	}

	Super::Render(renderTarget);
}

void Player::OnStay(Actor* other, const HitResult& hit)
{
	Vector pos = GetPos();
	bool isWall = (other->GetActorType() == ActorType::WALL);
	bool isGround = (other->GetActorType() == ActorType::Ground);
	bool isBoss = (other->GetActorType() == ActorType::Boss);
	bool isBossClone = (other->GetActorType() == ActorType::BossClone);

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

	if ((isBoss || isBossClone) && !_isInvincible)
	{
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

		TakeDamage(1, hitDirX);
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
	if (_isInvincible)
	{
		return;
	}
	if (_hp > 0)
	{
		_hp -= damage;
		_isInvincible = true;
		_invincibleTimer = 0.0f;
		this->GetComponent<FSMComponent>()->ChangeState("Hit");
	}

	if (_hp <= 0)
	{
		this->GetComponent<FSMComponent>()->ChangeState("Death");
	}
	_hitdirX = hitDirX;
}
