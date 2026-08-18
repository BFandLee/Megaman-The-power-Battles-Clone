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

#include "BossHpBarUI.h"
#include "UIManager.h"

void TestScene::loadResources()
{
	ResourceManager::GetInstance().LoadAllTexturesInDirectory(L"../Resources/sprites");
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
		BossHpBarUI* hpBar = new BossHpBarUI();
		hpBar->Init();
		hpBar->SetPos(Vector(450.0f, 605.0f)); // 화면 상단 위치
		hpBar->SetScale(Vector(0.7f, 0.7f));  // 도트 2배 확대
		hpBar->SetTarget(boss);
		UIManager::GetInstance().AddUI(hpBar);
	}
}
