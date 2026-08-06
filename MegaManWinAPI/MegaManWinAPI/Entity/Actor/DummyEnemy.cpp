#include "pch.h"
#include "DummyEnemy.h"
#include "BoxCollider.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "Texture.h"

DummyEnemy::DummyEnemy() : Super("DummyEnemy")
{
}

DummyEnemy::~DummyEnemy()
{
}

void DummyEnemy::Init()
{
	Super::Init();

	// 1. BoxCollider 생성 및 크기 설정
	BoxCollider* collider = AddComponent<BoxCollider>();
	collider->SetSize(50.0f, 50.0f);

	// 2. 보스의 Idle_1 이미지 로드 및 ImageRenderer 부착
	ImageRenderer* render = AddComponent<ImageRenderer>();
	Texture* tex = ResourceManager::GetInstance().GetTexture(L"Boss_Idle");
	render->SetTexture(tex);
}

void DummyEnemy::Update(float deltaTime)
{
	Super::Update(deltaTime);
}
