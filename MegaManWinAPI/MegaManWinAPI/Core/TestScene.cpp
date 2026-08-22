#include "pch.h"
#include "TestScene.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Background.h"
#include "Ground.h"
#include "WAllActor.h"
#include "Player.h"
#include "PlayerFactory.h"
#include "PlayerBullet.h"
#include "Boss.h"
#include "BossMissile.h"

#include "FSMComponent.h"
#include "AnimatorComponent.h"
#include "BossHpBarUI.h"
#include "PlayerHpBarUI.h"
#include "ReadyGoUI.h"
#include "ContinueUI.h"
#include "UIManager.h"

void TestScene::loadResources()
{
	ResourceManager::GetInstance().LoadAllTexturesInDirectory(L"Resources/sprites");
}

void TestScene::Init()
{
	Super::Init();

	_spawnCount = 1;
	_gameOverDelayTimer = 0.0f;
	_isWaitingGameOver = false;
	_gameClearDelayTimer = 0.0f;
	_isWaitingGameClear = false;

	SoundManager::GetInstance().PlayBGM(L"Gemini Man", true);
}

void TestScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	Boss* boss = static_cast<Boss*>(FindActorByType(ActorType::Boss));
	Player* player = static_cast<Player*>(FindActorByType(ActorType::Player));

	if (InputManager::GetInstance().GetButtonDown(KeyType::L))
	{
		if (boss != nullptr && player != nullptr)
		{
			// 플레이어 방향을 향해 대각선 위(-Y)로 레이저 조준
			float dirX = (player->GetPos().x >= boss->GetPos().x) ? 1.0f : -1.0f;
			Vector fireDir = Vector(dirX, -1.0f);
			fireDir.Normalize();
			BossMissile* laser = new BossMissile();
			laser->Init();
			laser->Fire(boss->GetPos(), fireDir, MissileType::Razer);
			AddActor(laser);
		}
	}

	// 플레이어가 파괴되어 씬에서 사라졌을 때
	if (player == nullptr)
	{
		// 스폰 횟수가 3회를 초과했을 경우 (최초 1회 + 부활 3회 = 총 4회 스폰 후 4번째 사망 시)
		if (_spawnCount >= _maxSpawnCount)
		{
			if (_playerHpBar) _playerHpBar->SetVisible(false);
			if (_bossHpBar) _bossHpBar->SetVisible(false);

			if (!_isWaitingGameOver)
			{
				_isWaitingGameOver = true;
				_gameOverDelayTimer = 0.0f;
			}
			else
			{
				_gameOverDelayTimer += deltaTime;
				if (_gameOverDelayTimer >= 1.0f)
				{
					SceneManager::GetInstance().ChangeScene(SceneType::GameOver);
				}
			}
		}
		// 아직 부활(Continue) 기회가 남아있고, 컨티뉴 UI가 아직 표시되지 않은 경우
		else if (_continueUI && !_continueUI->IsVisible())
		{
			if (_playerHpBar) _playerHpBar->SetVisible(false);
			if (_bossHpBar) _bossHpBar->SetVisible(false);
			_continueUI->StartCountdown(9);

			SoundManager::GetInstance().PlayBGM(L"Continue", true);
		}
	}

	// 보스가 파괴되어 씬에서 사라졌을 때 (클리어 처리)
	if (boss == nullptr && !_isWaitingGameOver)
	{
		if (!_isWaitingGameClear)
		{
			_isWaitingGameClear = true;
			_gameClearDelayTimer = 0.0f;

			if (_playerHpBar) _playerHpBar->SetVisible(false);
			if (_bossHpBar) _bossHpBar->SetVisible(false);

			if (player != nullptr)
			{
				player->SetInvincible(true);
			}
		}
		else
		{
			_gameClearDelayTimer += deltaTime;
			if (_gameClearDelayTimer >= 2.0f)
			{
				SceneManager::GetInstance().ChangeScene(SceneType::GameClear);
			}
		}
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

	// 천장 생성
	WallActor* ceiling = new WallActor();
	ceiling->Init();
	ceiling->SetActorType(ActorType::Ceiling);
	ceiling->SetPos(Vector(GWinSizeX / 2.0f, 0.0f));
	ceiling->SetSize((float)GWinSizeX, 50.0f);
	AddActor(ceiling);

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
	_continueUI->SetPos(Vector(10.0f, 0.0f));
	UIManager::GetInstance().AddUI(_continueUI);
	_continueUI->SetVisible(false);

	_continueUI->SetOnContinueCallback([this]()
		{
			// 1. SceneData.json의 초기 데이터 기반으로 플레이어 복원 스폰
			Actor* newPlayer = SpawnActorFromInitialData("Player");
			if (newPlayer != nullptr)
			{
				_spawnCount++; // 부활(재스폰) 횟수 증가
				AddActor(newPlayer);

				// 2. FSM을 Spawn 상태로 전환하여 등장 연출 후 조작 가능하도록 설정
				FSMComponent* playerFSM = newPlayer->GetComponent<FSMComponent>();
				if (playerFSM != nullptr)
				{
					playerFSM->ChangeState("Spawn");
				}

				// 부활 시에는 ReadyGoUI가 없으므로 착지 애니메이션 후 Idle 상태로 전환
				AnimatorComponent* pAnim = newPlayer->GetComponent<AnimatorComponent>();
				if (pAnim != nullptr)
				{
					pAnim->SetEndEvent(L"Spone", [newPlayer]() {
						Player* p = dynamic_cast<Player*>(newPlayer);
						if (p != nullptr)
						{
							FSMComponent* fsm = p->GetComponent<FSMComponent>();
							if (fsm != nullptr) fsm->ChangeState("Idle");
						}
					});
				}

				// 3. HP바에 새 플레이어 타겟 연결 및 UI 다시 보이기
				_playerHpBar->SetTarget(static_cast<Player*>(newPlayer));
				_playerHpBar->SetVisible(true);
				_bossHpBar->SetVisible(true);
			}

			// 4. 컨티뉴 UI 숨기기
			_continueUI->SetVisible(false);

			SoundManager::GetInstance().PlayBGM(L"Gemini Man", true);
		});

	_continueUI->SetOnGameOverCallback([this]()
		{
			// 게임 오버 씬 전환
			SceneManager::GetInstance().ChangeScene(SceneType::GameOver);
		});
}
