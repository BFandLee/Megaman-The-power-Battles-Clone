#include "pch.h"
#include "TestScene.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "Background.h"
#include "Ground.h"
#include "WAllActor.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "DummyEnemy.h"

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
	Player* player = new Player();
	player->Init();
	player->SetPos(Vector(GWinSizeX / 2, GWinSizeY / 2));

	// 생성했으면 무조건 초기화
	bg->Init();
	ground->Init();
	

	AddActor(bg);
	AddActor(ground);
	AddActor(player);

	// 총알
	PlayerBullet* bullet = new PlayerBullet();
	bullet->SetPos(player->GetPos());

	// 테스트 샌드백
	DummyEnemy* dummy = new DummyEnemy();
	dummy->Init();
	dummy->SetPos(Vector(400, 300));
	AddActor(dummy);
}
