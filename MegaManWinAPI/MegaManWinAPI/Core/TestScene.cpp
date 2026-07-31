#include "pch.h"
#include "TestScene.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "Background.h"


void TestScene::loadResources()
{
	ResourceManager::GetInstance().LoadTexture(
		L"Background",
		L"Background\\background\\sprite_0000.png"
	);
}

void TestScene::createObjects()
{
	// 배경 액터 생성
	Actor* bg = new Background();

	// 생성했으면 무조건 초기화
	bg->Init();

	AddActor(bg);
}
