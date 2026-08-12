#include "pch.h"
#include "Boss.h"
#include "FSMComponent.h"
#include "BoxCollider.h"
#include "AnimatorComponent.h"
#include "BossSpawn.h"
#include "BossPhase1State.h"
#include "BossPhase2State.h"
#include "BossDead.h"
// TODO: BoxCollider, ImageRenderer 등 필요한 컴포넌트 헤더를 추가하세요.

Boss::Boss() : Super("Boss")
{
}

Boss::~Boss()
{
}

void Boss::Init()
{
	Super::Init();

	// Component 추가
	_fsm = AddComponent<FSMComponent>();
    BoxCollider* box = AddComponent<BoxCollider>();
    AnimatorComponent* animator = AddComponent<AnimatorComponent>();
	
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

void Boss::SetInvincible(bool isInvincible)
{
    // TODO: 보스의 무적 상태를 변경합니다.
    _isInvincible = isInvincible;
}

void Boss::PullPlayer()
{
    // TODO: 플레이어의 위치를 찾아 보스 쪽으로 당기는 힘(속도)을 가하세요.
}

void Boss::ActivateShield()
{
    // TODO: 마그넷 실드 활성화 로직 (투사체 반사 등)
}

void Boss::JumpTo(Vector dest)
{
    // TODO: 목표 위치(dest)를 향해 포물선 점프 이동을 시작하세요.
}

void Boss::ShootMagnetMissile()
{
    // TODO: 마그넷 미사일 객체를 생성(스폰)하여 발사하세요.
}
