#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Game.h"
#include "Scene.h"
#include "Player.h"

void UIManager::Init()
{
	_hpTexture = ResourceManager::GetInstance().GetTexture(L"PlayerHP");
}

void UIManager::Update(float deltaTime)
{
}

void UIManager::Render(HDC hdc)
{
	if (_hpTexture == nullptr)
		return;

	Scene* scene = Game::GetInstance().GetScene();
	if (scene == nullptr)
		return;

	Player* player = scene->GetPlayer();
	if (player == nullptr)
		return;

	int32 hp = player->GetHp();
	if (hp < 0)
		hp = 0;

	// 1 heart = 10 HP. Max HP = 100 -> Max Hearts = 10.
	int32 heartCount = hp / 10;

	Vector startPos(100.f, 100.f);
	float spacing = 5.f;
	
	// Disable center alignment to draw based on top-left (100, 100)
	_hpTexture->SetApplyCenter(false);

	float heartWidth = (float)_hpTexture->GetFrameSize().cx;

	for (int32 i = 0; i < heartCount; ++i)
	{
		Vector drawPos = startPos;
		drawPos.x += i * (heartWidth + spacing);
		_hpTexture->RenderScreen(hdc, drawPos);
	}
}
