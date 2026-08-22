#include "pch.h"
#include "Boss.h"
#include "FSMComponent.h"
#include "BoxCollider.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "BossSpawn.h"
#include "BossPhase1State.h"
#include "BossPhase2State.h"
#include "BossDead.h"
#include "BossBlackboard.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include "SoundManager.h"

Boss::Boss() : Super("Boss")
{
}

Boss::~Boss()
{
	delete _bb;
	if (_bb != nullptr)
	{
		_bb = nullptr;
	}
}

void Boss::Init()
{
	Super::Init();

	// Component 추가
	_fsm = AddComponent<FSMComponent>();
	_bb = new BossBlackboard();
    BoxCollider* box = AddComponent<BoxCollider>();
    AnimatorComponent* animator = AddComponent<AnimatorComponent>();
	RigidBodyComponent* rigid = AddComponent<RigidBodyComponent>();
	
	_bb->Init(this);
	box->SetSize(60, 60);
	
	// Animation 폴더 안의 모든 파일을 순회
	std::filesystem::path animPath = "Resources/sprites/Boss/Animation/";
	if (std::filesystem::exists(animPath) && std::filesystem::is_directory(animPath))
	{
		for (const auto& entry : std::filesystem::directory_iterator(animPath))
		{
			// 확장자가 .json 인 경우에만
			if (entry.path().extension() == ".json")
			{
				std::wstring stateName = entry.path().stem().wstring(); // 파일 이름만 추출 (예: idle.json -> idle)
				std::wstring filePath = entry.path().wstring();         // 전체 경로

				animator->LoadAnimationFromJson(stateName, filePath);
			}
		}
	}
	
	animator->SetEndEvent(L"Hit", [this]()
		{
			_isHit = false;
		});

	_fsm->AddState("Spawn", new BossSpawn(_fsm));
	_fsm->AddState("Phase1", new BossPhase1State(_fsm));
	_fsm->AddState("Phase2", new BossPhase2State(_fsm));
	_fsm->AddState("Dead", new BossDead(_fsm));
	
	_fsm->ChangeState("Spawn");
}

void Boss::Update(float deltaTime)
{
	if (_bb == nullptr)
	{
		return;
	}

	if (_bb != nullptr)
	{
		_bb->Update(deltaTime);
	}

	if (_isInvincible)
	{
		_invincibleTimer += deltaTime;

		if (_invincibleTimer > 2.0f)
		{
			_isInvincible = false;
		}
	}

	Super::Update(deltaTime);
}

void Boss::Render(ID2D1RenderTarget* renderTarget)
{
	// 무적 상태일 때만 특정 조건에서 return 시켜서 렌더링을 건너뜀 (0.2초 단위로 깜빡임)
	if (_isInvincible)
	{
		if ((int32)(_invincibleTimer * 5) % 2)
			return;
	}

	Super::Render(renderTarget);
}

void Boss::TakeDamage(int damage, float hitDirX)
{
	if (_isInvincible) return;

	if (_hp > 0)
	{
		_hp -= damage;
		_isInvincible = true;
		_invincibleTimer = 0.0f;
		_hitDirX = hitDirX;

		SoundManager::GetInstance().PlaySFX(L"geminiman_hit");

		if (!_isSuperArmor)
		{
			_isHit = true;

			AnimatorComponent* animator = GetComponent<AnimatorComponent>();
			if (animator != nullptr)
			{
				animator->Play(L"Hit");
			}

			RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
			if (rigid != nullptr)
			{
				rigid->SetGrounded(false);
				rigid->SetVelocity(Vector(hitDirX * BOSS_KNOCKBACK, BOSS_RESTORING));
			}
		}
	}
	
	if (_hp <= 0)
	{
		this->GetComponent<FSMComponent>()->ChangeState("Dead");
	}
}

void Boss::OnStay(Actor* other, const HitResult& hit)
{
	Vector pos = GetPos();
	bool isWall = (other->GetActorType() == ActorType::WALL);
	bool isGround = (other->GetActorType() == ActorType::Ground);
	bool isPlayerBullet = (other->GetActorType() == ActorType::PlayerBullet);

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
			float pushOut = std::max(overlapY - 0.1f, 0.0f);

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

	SetPos(pos);
}

void Boss::OnExit(Actor* other)
{
	RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
	if (other->GetActorType() == ActorType::Ground)
	{
		rigid->SetGrounded(false);
	}
}

void Boss::SetInvincible(bool isInvincible)
{
    // TODO: 보스의 무적 상태를 변경합니다.
    _isInvincible = isInvincible;
}

void Boss::SetLookDirX(float dir)
{
	TransformComponent* transform = GetComponent<TransformComponent>();

	if (transform)
	{
		Vector currentScale = transform->GetScale();
		currentScale.x = abs(currentScale.x) * (-dir);
		transform->SetScale(currentScale);
	}
}