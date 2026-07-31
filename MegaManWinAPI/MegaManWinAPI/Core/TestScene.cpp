#include "pch.h"
#include "TestScene.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "Background.h"
#include "Ground.h"
#include "WAllActor.h"

void TestScene::loadResources()
{
	ResourceManager::GetInstance().LoadTexture(
		L"Background",
		L"Background\\background\\sprite_0000.png"
	);
	ResourceManager::GetInstance().LoadTexture(
		L"Ground",
		L"Background\\background\\sprite_0001.png"
	);
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

	// 생성했으면 무조건 초기화
	bg->Init();
	ground->Init();

	AddActor(bg);
	AddActor(ground);
}
