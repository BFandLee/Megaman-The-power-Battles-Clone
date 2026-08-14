#include "pch.h"
#include "BossClone.h"
#include "AnimatorComponent.h"
#include "BoxCollider.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "Boss.h"


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
	TransformComponent* transform = GetComponent<TransformComponent>();

	Vector scale = _ownerBoss->GetComponent<TransformComponent>()->GetScale();
	Vector pos = _ownerBoss->GetComponent<TransformComponent>()->GetPos();
	float modifier = 400.0f;
	transform->SetScale(scale);
	transform->SetPos(Vector(pos.x - modifier, pos.y));

	Vector bossVelocity = _ownerBoss->GetComponent<RigidBodyComponent>()->GetVelocity();
	rigid->SetVelocity(bossVelocity);

	wstring state = _ownerBoss->GetComponent<AnimatorComponent>()->GetCurrentClipName();
	animator->Play(state);

}

void BossClone::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}

void BossClone::TakeDamage(float damage, float hitDirX)
{
	if (_ownerBoss != nullptr)
	{
		_ownerBoss->TakeDamage(damage, hitDirX);
	}
}

void BossClone::OnStay(Actor* other, const HitResult& hit)
{
	Vector pos = GetPos();
	bool isGround = (other->GetActorType() == ActorType::Ground);

	BoxCollider* myCol = GetComponent<BoxCollider>();
	BoxCollider* otherCol = other->GetComponent<BoxCollider>();
	if (!myCol || !otherCol) return;

	Vector myColPos = myCol->GetColliderPos();
	Vector otherColPos = otherCol->GetColliderPos();

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

void BossClone::OnExit(Actor* other)
{
	RigidBodyComponent* rigid = GetComponent<RigidBodyComponent>();
	if (other->GetActorType() == ActorType::Ground)
	{
		rigid->SetGrounded(false);
	}
}
