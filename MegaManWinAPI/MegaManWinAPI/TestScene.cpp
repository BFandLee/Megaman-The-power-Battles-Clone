#include "pch.h"
#include "TestScene.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "Background.h"


void TestScene::loadResources()
{
	ResourceManager::GetInstance().LoadTexture(L"Map", L"background.png");
}

void TestScene::createObjects()
{
	// 배경 액터 생성
	Actor* bg = new Background();

	// 컴포넌트 부착
	ImageRenderer* renderer = bg->AddComponent<ImageRenderer>();

	renderer->Init(L"Background_key");

	AddActor(bg);
}
