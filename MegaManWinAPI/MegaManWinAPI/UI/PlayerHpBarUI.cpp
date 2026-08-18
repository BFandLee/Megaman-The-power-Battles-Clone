#include "pch.h"
#include "PlayerHpBarUI.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Player.h"


void PlayerHpBarUI::Init()
{
	_hpSheetTexture = ResourceManager::GetInstance().GetTexture(L"PlayerUI_Hpbar_Sheet");
	_hitBarTexture = ResourceManager::GetInstance().GetTexture(L"PlayerUi_HitBar_Sheet");
	_characterSheetTexture = ResourceManager::GetInstance().GetTexture(L"PlayerUI_Character_Sheet");
	_playerNameTexture = ResourceManager::GetInstance().GetTexture(L"PlayerUI_playername_Sheet");

	_displayHpRatio = 0.0f;
	_isSpawning = true;
	if (_player)
	{
		_prevHp = _player->GetHp();
	}
}

void PlayerHpBarUI::Update(float deltaTime)
{
	if (!_player) return;
	float targetRatio = (float)_player->GetHp() / (float)_player->GetMaxHp();

	if (_isSpawning)
	{
		_displayHpRatio += _fillSpeed * deltaTime;
		if (_displayHpRatio >= targetRatio)
		{
			_displayHpRatio = targetRatio;
			_isSpawning = false;
		}
	}
	else
	{
		_displayHpRatio = targetRatio;

		if (_displayHpRatio <= 0.25f)
		{
			_blinkTimer += deltaTime;
			if (_blinkTimer > 0.15f)
			{
				_isBlinkVisible = !_isBlinkVisible;
				_blinkTimer = 0.0f;
			}
		}
		else
		{
			_isBlinkVisible = false;
			_blinkTimer = 0.0f;
		}
		UpdateHitBlink(deltaTime);
	}
}

void PlayerHpBarUI::Render(ID2D1RenderTarget* renderTarget)
{
	if (!_hpSheetTexture) return;
	if (!_hitBarTexture) return;
	if (!renderTarget) return;

	Vector pos = GetPos();
	D2D1_RECT_F srcRect = D2D1::RectF(0.0f, 0.0f, 162.0f, 121.0f);
	D2D1_RECT_F destRect = D2D1::RectF(pos.x, pos.y, pos.x + 162.0f, pos.y + 121.0f);
	renderTarget->DrawBitmap(
		_hpSheetTexture->GetBitmap(),
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		srcRect
	);
	DrawPortrait(renderTarget);
	DrawPlayerName(renderTarget);

	int currentSteps = static_cast<int>(_displayHpRatio * 16.0f + 0.5f);
	for (int i = 0; i < 8; ++i)
	{
		int segementValue = currentSteps - (i * 2);
		
		DrawSegment(renderTarget, i, segementValue);
	}
	if (_displayHpRatio <= 0.25f && _isBlinkVisible)
	{
		D2D1_RECT_F hitsrc = D2D1::RectF(0.0f, 0.0f, 162.0f, 123.0f);
		D2D1_RECT_F hitdest = D2D1::RectF(pos.x, pos.y, pos.x + 162.0f, pos.y + 123.0f);
		renderTarget->DrawBitmap(
			_hitBarTexture->GetBitmap(),
			hitdest,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			hitsrc
		);
	}

}

void PlayerHpBarUI::UpdateHitBlink(float deltaTime)
{
	int currentHp = _player->GetHp();
	if (currentHp < _prevHp)
	{
		_hitBlinkDuration = 0.8f;
		_hitBlinkTimer = 0.0f;
		_isHitRed = true;
	}

	if (_hitBlinkDuration > 0.0f)
	{
		_hitBlinkDuration -= deltaTime;
		_hitBlinkTimer += deltaTime;

		if (_hitBlinkTimer >= 0.1f)
		{
			_isHitRed = !_isHitRed;
			_hitBlinkTimer = 0.0f;
		}

		if (_hitBlinkDuration <= 0.0f)
		{
			_isHitRed = false;
		}
	}

	_prevHp = currentHp;
}

void PlayerHpBarUI::DrawPortrait(ID2D1RenderTarget* rt)
{
	if (!_characterSheetTexture) return;

	D2D1_RECT_F srcRect = D2D1::RectF(0.0f, 0.0f, 64.0f, 64.0f);
	if (_isHitRed)
	{
		srcRect = D2D1::RectF(64.0f, 0.0f, 128.0f, 64.0f);
	}

	Vector pos = GetPos();

	D2D1_RECT_F destRect = D2D1::RectF(
		pos.x + 49.0f, 
		pos.y + 28.5f, 
		pos.x + 49.0f + 64.0f,
		pos.y + 28.5f + 64.0f
	);

	rt->DrawBitmap(
		_characterSheetTexture->GetBitmap(),
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		srcRect
	);

}

void PlayerHpBarUI::DrawPlayerName(ID2D1RenderTarget* rt)
{
	if (!_playerNameTexture) return;
	
	Vector pos = GetPos();

	D2D1_RECT_F srcRect = D2D1::RectF(0.0f, 0.0f, 214.0f, 64.0f);


	D2D1_RECT_F destRect = D2D1::RectF(
		pos.x + 170.0f,
		pos.y + 50.0f,
		pos.x + 170.0f + 214.0f,
		pos.y + 50.0f + 64.0f
	);

	rt->DrawBitmap(
		_playerNameTexture->GetBitmap(),
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		srcRect
	);
}

void PlayerHpBarUI::DrawSegment(ID2D1RenderTarget* rt, int segmentIndex, int segmentValue)
{
	if (segmentValue <= 0) return;

	D2D1_RECT_F srcRect = SEGMENT_RECTS[segmentIndex];

	if (segmentValue == 1)
	{
		srcRect.left  += 162.0f;
		srcRect.right += 162.0f;
	}
	else if (segmentValue >= 2)
	{
		srcRect.left  += 323.f;
		srcRect.right += 323.f;
	}

	Vector pos = GetPos();
	D2D1_RECT_F destRect = D2D1::RectF(
		pos.x + SEGMENT_RECTS[segmentIndex].left,
		pos.y + SEGMENT_RECTS[segmentIndex].top,
		pos.x + SEGMENT_RECTS[segmentIndex].right,
		pos.y + SEGMENT_RECTS[segmentIndex].bottom
	);
	// 4. 잘라낸 조각을 화면에 렌더링!
	rt->DrawBitmap(
		_hpSheetTexture->GetBitmap(),
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		srcRect
	);
}
