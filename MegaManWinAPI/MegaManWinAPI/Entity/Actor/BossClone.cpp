#include "pch.h"
#include "BossClone.h"
#include "AnimatorComponent.h"
#include "BoxCollider.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "FSMComponent.h"
#include "Boss.h"
#include "SceneManager.h"
#include "Scene.h"


BossClone::BossClone() : Super("BossClone")
{
}

BossClone::~BossClone()
{
}

void BossClone::Init()
{
	Super::Init();

	AnimatorComponent* animator = AddComponent<AnimatorComponent>();
	BoxCollider* collider = AddComponent<BoxCollider>();
	RigidBodyComponent* rigid = AddComponent<RigidBodyComponent>();
	AddComponent<TransformComponent>();
	AddComponent<FSMComponent>();

	collider->SetSize(60, 60);

	// Animation 폴더 안의 모든 파일을 순회
	for (const auto& entry : std::filesystem::directory_iterator("../Resources/sprites/Boss/Animation/"))
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

void BossClone::Update(float deltaTime)
{
	Super::Update(deltaTime);

	RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
	AnimatorComponent* animator = GetComponent<AnimatorComponent>();
	BoxCollider* collider = GetComponent<BoxCollider>();

	Vector bossVelocity = _ownerBoss->GetComponent<RigidBodyComponent>()->GetVelocity();
	rigid->SetVelocity(Vector(-bossVelocity.x, bossVelocity.y));

	collider->SetSize(_ownerBoss->GetComponent<BoxCollider>()->GetWidth(), _ownerBoss->GetComponent<BoxCollider>()->GetHeight());

	wstring state = _ownerBoss->GetComponent<AnimatorComponent>()->GetCurrentClipName();
	animator->Play(state);

	Actor* player = SceneManager::GetInstance().GetScene()->FindActorByType(ActorType::Player);
	if (player != nullptr)
	{
		float dirX = (player->GetPos().x >= GetPos().x) ? 1.0f : -1.0f;
		SetLookDirX(dirX);
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

void BossClone::Render(ID2D1RenderTarget* renderTarget)
{
	if (_isInvincible)
	{
		if ((int32)(_invincibleTimer * 10) % 2)
			return;
	}

	Super::Render(renderTarget);
}

void BossClone::TakeDamage(int damage, float hitDirX)
{
	if (_isInvincible) return;
	if (_ownerBoss == nullptr) return;

	float hp = _ownerBoss->GetHP();
	if (hp > 0)
	{
		hp -= damage;
		_isInvincible = true;
		_invincibleTimer = 0.0f;
		this->GetComponent<AnimatorComponent>()->Play(L"Hit");
	}

	if (hp <= 0)
	{
		this->GetComponent<FSMComponent>()->ChangeState("Dead");
	}
	if (_ownerBoss != nullptr)
	{
		_ownerBoss->TakeDamage(damage, hitDirX);
	}
}

void BossClone::OnStay(Actor* other, const HitResult& hit)
{
	Vector pos = GetPos();
	bool isWall = (other->GetActorType() == ActorType::WALL);
	bool isGround = (other->GetActorType() == ActorType::Ground);
	bool isPlayer = (other->GetActorType() == ActorType::Player);
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

	if ((isPlayer || isPlayerBullet))
	{
		float diffX = this->GetPos().x - other->GetPos().x;

		// 방향벡터만 남기기 위해서 정규화
		float hitDirX = 0.0f;
		if (diffX > 0)
		{
			hitDirX = 1.0f;
		}
		else if (diffX < 0)
		{
			hitDirX = -1.0f;
		}

		TakeDamage(5, hitDirX);
	}

	SetPos(pos);
}

void BossClone::OnExit(Actor* other)
{
	RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
	if (other->GetActorType() == ActorType::Ground)
	{
		rigid->SetGrounded(false);
	}
}

void BossClone::SetLookDirX(float dir)
{
	TransformComponent* transform = GetComponent<TransformComponent>();

	if (transform)
	{
		Vector currentScale = transform->GetScale();
		currentScale.x = abs(currentScale.x) * (-dir);
		transform->SetScale(currentScale);
	}
}
