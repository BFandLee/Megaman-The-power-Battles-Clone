#pragma once
#include "UIElement.h"

const D2D1_RECT_F SEGMENT_RECTS[8] = {
	D2D1::RectF(28.0f,  5.0f,  77.0f,  27.0f),  // 0번째 사각형 칸
	D2D1::RectF(6.0f, 25.0f,  32.0f,  57.0f),   // 1번째 사각형 칸
	D2D1::RectF(6.0f, 63.0f,  32.0f,  95.0f),   // 2번째 사각형 칸
	D2D1::RectF(28.0f, 93.0f,  77.0f, 115.0f),  // 3번째 사각형 칸
	D2D1::RectF(83.0f, 93.0f, 132.0f, 115.0f),  // 4번째 사각형 칸
	D2D1::RectF(128.0f, 63.0f, 154.0f, 95.0f),  // 5번째 사각형 칸
	D2D1::RectF(128.0f, 25.0f, 154.0f, 57.0f),  // 6번째 사각형 칸
	D2D1::RectF(83.0f,  5.0f, 132.0f,  27.0f)   // 7번째 사각형 칸
};
class Texture;

class PlayerHpBarUI : public UIElement
{
public:
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	void SetTarget(class Player* player) { _player = player; }
private:
	class Player* _player = nullptr;
	Texture* _hpSheetTexture = nullptr;
	Texture* _hitBarTexture = nullptr;
	Texture* _characterSheetTexture = nullptr;
	Texture* _playerNameTexture = nullptr;

	float _displayHpRatio = 0.0f;
	float _blinkTimer = 0.0f;
	float _fillSpeed = 0.8f;
	bool _isBlinkVisible = false;
	bool _isSpawning = false;

	int _prevHp = 0;
	float _hitBlinkDuration = 0.0f;
	float _hitBlinkTimer = 0.0f;
	bool _isHitRed = false;

	void UpdateHitBlink(float deltaTime);
	void DrawPortrait(ID2D1RenderTarget* rt);
	void DrawPlayerName(ID2D1RenderTarget* rt);
	void DrawSegment(ID2D1RenderTarget* rt, int segmentIndex, int segmentValue);
};

