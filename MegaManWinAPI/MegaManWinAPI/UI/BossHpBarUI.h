#pragma once
#include "UIElement.h"

// 64x64 시트 내 파츠별 실제 픽셀 영역 정의
const D2D1_RECT_F HEAD_SRC_RECT = D2D1::RectF(4.0f, 7.0f, 60.0f, 57.0f);   // 너비: 56px, 높이: 50px
const D2D1_RECT_F BODY_SRC_RECT = D2D1::RectF(92.0f, 7.0f, 100.0f, 57.0f);  // 너비: 8px,  높이: 50px
const D2D1_RECT_F TAIL_SRC_RECT = D2D1::RectF(147.0f, 7.0f, 173.0f, 57.0f); // 너비: 26px, 높이: 50px

const float HEAD_WIDTH = 56.0f;
const float BODY_WIDTH = 8.0f;
const float TAIL_WIDTH = 26.0f;
const float BAR_HEIGHT = 50.0f;

class BossHpBarUI : public UIElement
{
public:
	void SetTarget(class Boss* boss) { _boss = boss; }

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	void DrawPart(ID2D1RenderTarget* rt, HpBarPart part, Vector destPos, bool isFilled);
private:
	class Boss* _boss = nullptr;
	class Texture* _hpTexture = nullptr;
	class Texture* _hitHpTexture = nullptr;
	float _displayHpRatio = 0.0f;
	float _fillSpeed = 0.8f;
	bool _isSpawning = true;
	int _maxBlocks = 28;
	
};

