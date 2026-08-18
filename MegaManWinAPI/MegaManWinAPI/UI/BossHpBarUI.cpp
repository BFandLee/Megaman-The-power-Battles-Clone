#include "pch.h"
#include "BossHpBarUI.h"
#include "Boss.h"
#include "Texture.h"
#include "ResourceManager.h"

void BossHpBarUI::Init()
{
    _hpTexture = ResourceManager::GetInstance().GetTexture(L"BossHp_Hp_Sheet");
    _hitHpTexture = ResourceManager::GetInstance().GetTexture(L"BossHp_HitHp_Sheet");

	_displayHpRatio = 0.0f;
	_isSpawning = true;
}

void BossHpBarUI::Update(float deltaTime)
{
	if (_boss == nullptr) return;
	float targetRatio = _boss->GetHP() / _boss->GetMaxHP();

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
	}
}

void BossHpBarUI::Render(ID2D1RenderTarget* renderTarget)
{
	if (!_hpTexture) return;
	if (!_hitHpTexture) return;
	if (!renderTarget) return;

	Vector pos = GetPos();
	Vector scale = GetScale();
	int currentBlocks = static_cast<int>(std::round(_maxBlocks * _displayHpRatio));

    // Draw head
    DrawPart(renderTarget, HpBarPart::Head, pos, currentBlocks > 0);

    // Draw Body
    for (int i = 0; i < _maxBlocks; ++i)
    {
        Vector blockPos(pos.x + (HEAD_WIDTH + i * BODY_WIDTH) * scale.x, pos.y);
        DrawPart(renderTarget, HpBarPart::Body, blockPos, i < currentBlocks);
    }

    // Draw Tail
    Vector tailPos(pos.x + (HEAD_WIDTH + _maxBlocks * BODY_WIDTH) * scale.x, pos.y);
    DrawPart(renderTarget, HpBarPart::Tail, tailPos, currentBlocks == _maxBlocks);
}

void BossHpBarUI::DrawPart(ID2D1RenderTarget* rt, HpBarPart part, Vector destPos, bool isFilled)
{
    // 1. isFilled 여부에 따라 초록 시트 or 보라 시트 텍스처 선택
    Texture* tex = isFilled ? _hpTexture : _hitHpTexture;
    if (tex == nullptr) return;

    D2D1_RECT_F srcRect;
    float width = 0.0f;

    // 2. switch-case로 파츠별 원본 영역(srcRect)과 너비 지정
    switch (part)
    {
    case HpBarPart::Head:
        srcRect = HEAD_SRC_RECT;
        width = HEAD_WIDTH;
        break;
    case HpBarPart::Body:
        srcRect = BODY_SRC_RECT;
        width = BODY_WIDTH;
        break;
    case HpBarPart::Tail:
        srcRect = TAIL_SRC_RECT;
        width = TAIL_WIDTH;
        break;
    }

    // 3. 목적지 사각형 계산 후 DrawBitmap 호출
    Vector scale = GetScale();
    D2D1_RECT_F destRect = D2D1::RectF(
        destPos.x,
        destPos.y,
        destPos.x + width * scale.x,
        destPos.y + BAR_HEIGHT * scale.y
    );

    rt->DrawBitmap(tex->GetBitmap(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
}
