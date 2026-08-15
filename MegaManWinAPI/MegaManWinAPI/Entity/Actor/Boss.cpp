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
	
	_bb->OwnerBoss = this;
	_bb->BossRigidBody = rigid;
	box->SetSize(60, 60);
	
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

	if (_bb != nullptr && _bb->TargetPlayer == nullptr)
	{
		Actor* player = SceneManager::GetInstance().GetScene()->FindActorByType(ActorType::Player);
		if (player != nullptr)
		{
			_bb->TargetPlayer = player;
			_bb->PlayerTransform = player->GetComponent<TransformComponent>();
			_bb->BossTransform = this->GetComponent<TransformComponent>();
		}
	}
	Super::Update(deltaTime);
}

void Boss::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}

void Boss::TakeDamage(float damage, float hitDirX)
{
	if (_isInvincible) return;

    // TODO: 보스가 데미지를 입었을 때 체력을 감소시키는 로직을 작성하세요.
	if (_hp > 0)
	{
		_hp -= damage;
		_isInvincible = true;
	}
	
	if (_hp <= 0)
	{
		this->GetComponent<FSMComponent>()->ChangeState("Dead");
	}
	// _hitdirX = hitDirX;

    // HP가 0 이하가 될 때의 처리는 FSM 업데이트에서 처리하거나 여기서 FSM 상태를 즉시 변경할 수 있습니다.
}

void Boss::OnStay(Actor* other, const HitResult& hit)
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
