#include "pch.h"
#include "TestScene.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "Background.h"
#include "Ground.h"
#include "WAllActor.h"
#include "Player.h"
#include "PlayerFactory.h"
#include "PlayerBullet.h"
#include "Boss.h"

#include "FSMComponent.h"
#include "BossHpBarUI.h"
#include "PlayerHpBarUI.h"
#include "ReadyGoUI.h"
#include "ContinueUI.h"
#include "UIManager.h"

void TestScene::loadResources()
{
	ResourceManager::GetInstance().LoadAllTexturesInDirectory(L"../Resources/sprites");
}

void TestScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	Player* player = static_cast<Player*>(FindActorByType(ActorType::Player));

	// 플레이어가 파괴되어 사라졌고, 컨티뉴 UI가 아직 실행 전이라면
	if (player == nullptr && _continueUI && !_continueUI->IsVisible())
	{
		_playerHpBar->SetVisible(false);
		_bossHpBar->SetVisible(false);
		_continueUI->StartCountdown(9);
	}
}

void TestScene::createObjects()
{
	// 배경 액터 생성
	Actor* bg = new Background();
	Actor* ground = new Ground();

	// 양 사이드를 막는 벽 생성
	WallActor* leftwall = new WallActor();
	leftwall->Init();
	leftwall->SetPos(Vector(0, GWinSizeY/2));
	leftwall->SetSize(50, GWinSizeY);
	AddActor(leftwall);

	WallActor* rightwall = new WallActor();
	rightwall->Init();
	rightwall->SetPos(Vector(GWinSizeX, GWinSizeY / 2));
	rightwall->SetSize(50, GWinSizeY);
	AddActor(rightwall);

	// 플레이어
	ActorFactory* factory = new PlayerFactory();
	Actor* player = factory->CreateActor(Vector(GWinSizeX / 2, 0));
	delete factory;

	// 생성했으면 무조건 초기화
	bg->Init();
	ground->Init();
	

	AddActor(bg);
	AddActor(ground);
	AddActor(player);

	// 총알
	PlayerBullet* bullet = new PlayerBullet();
	bullet->SetPos(player->GetPos());

	// 보스
	Boss* boss = new Boss();
	boss->Init();
	boss->SetPos(Vector(600, 300));
	AddActor(boss);
}

void TestScene::createUI()
{
	// 1. 씬에 등록된 액터 중 Boss 탐색
	Boss* boss = static_cast<Boss*>(FindActorByType(ActorType::Boss));
	
	// 2. 보스 체력 바 생성 및 등록
	if (boss != nullptr)
	{
		_bossHpBar = new BossHpBarUI();
		_bossHpBar->Init();
		_bossHpBar->SetPos(Vector(450.0f, 605.0f)); // 화면 상단 위치
		_bossHpBar->SetScale(Vector(0.7f, 0.7f));  // 도트 2배 확대
		_bossHpBar->SetTarget(boss);
		UIManager::GetInstance().AddUI(_bossHpBar);
	}

	Player* player = static_cast<Player*>(FindActorByType(ActorType::Player));

	// 2. 보스 체력 바 생성 및 등록
	if (player != nullptr)
	{
		_playerHpBar = new PlayerHpBarUI();
		_playerHpBar->SetPos(Vector(40.0f, 40.0f)); // 화면 상단 위치
		_playerHpBar->SetTarget(player);
		_playerHpBar->Init();
		UIManager::GetInstance().AddUI(_playerHpBar);
	}

	ReadyGoUI* readyGoUI = new ReadyGoUI();
	readyGoUI->Init();
	readyGoUI->SetOnFinishedCallback([this, player, boss]() {
		
		if (boss != nullptr)
		{
			FSMComponent* bossFSM = boss->GetComponent<FSMComponent>();
			if (bossFSM != nullptr)
			{
				bossFSM->ChangeState("Phase1");
			}
		}

		if (player != nullptr)
		{
			FSMComponent* playerFSM = player->GetComponent<FSMComponent>();
			if (playerFSM != nullptr)
			{
				playerFSM->ChangeState("Idle");
			}
		}

		});
	UIManager::GetInstance().AddUI(readyGoUI);

	_continueUI = new ContinueUI();
	_continueUI->Init();
	UIManager::GetInstance().AddUI(_continueUI);
	_continueUI->SetVisible(false);

	_continueUI->SetOnContinueCallback([this]()
		{
			// 1. SceneData.json의 초기 데이터 기반으로 플레이어 복원 스폰
			Actor* newPlayer = SpawnActorFromInitialData("Player");
			if (newPlayer != nullptr)
			{
				AddActor(newPlayer);

				// 2. FSM을 Idle 상태로 전환하여 즉시 조작 가능하도록 설정
				FSMComponent* playerFSM = newPlayer->GetComponent<FSMComponent>();
				if (playerFSM != nullptr)
				{
					playerFSM->ChangeState("Spone");
				}

				// 3. HP바에 새 플레이어 타겟 재연결 및 UI 다시 보이기
				_playerHpBar->SetTarget(static_cast<Player*>(newPlayer));
				_playerHpBar->SetVisible(true);
				_bossHpBar->SetVisible(true);
			}

			// 4. 컨티뉴 UI 숨기기
			_continueUI->SetVisible(false);
		});

	_continueUI->SetOnGameOverCallback([this]()
		{
			// 게임 오버 씬 전환
		});
}
