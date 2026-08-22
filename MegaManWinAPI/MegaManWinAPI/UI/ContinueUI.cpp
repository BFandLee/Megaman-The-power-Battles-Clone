#include "pch.h"
#include "ContinueUI.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Texture.h"

void ContinueUI::Init()
{

	_smallSheetTexture = ResourceManager::GetInstance().GetTexture(L"Continue_small_Sheet");
	_bigSheetTexture = ResourceManager::GetInstance().GetTexture(L"Continue_Big_Sheet");
}

void ContinueUI::Update(float deltaTime)
{
	if (_state == ContinueState::Counting)
	{
		if (InputManager::GetInstance().GetButtonDown(KeyType::ENTER))
		{
			_state = ContinueState::Continued;
			if(_onContinue) _onContinue();
		}

		_timer += deltaTime;
		if (_timer >= 1.0f)
		{
			_timer = 0.0f;
			_currentCount--;
			if (_currentCount < 0)
			{
				_state = ContinueState::TimeOut;
				if(_onGameOver) _onGameOver();
			}
		}
	}

	if (_smallSheetTexture)
	{
		_smallSheetTexture->SetApplyCenter(false);
	}
}

void ContinueUI::Render(ID2D1RenderTarget* renderTarget)
{
	if (!renderTarget || _state != ContinueState::Counting) return;
	if (!_smallSheetTexture || !_bigSheetTexture) return;

	// SmallContinue
	Vector basePos = GetPos();
	Vector scale = Vector(1.0f, 1.0f);

	float padding = 15.0f;
	float offsetX = (100.0f + padding) * scale.x;
	
	Vector textPos = basePos;
	Vector textSrcPos = Vector(0.0f, 0.0f);
	Vector textSize = Vector(135.0f, 64.0f);
	_smallSheetTexture->Render(renderTarget, textPos, textSrcPos, textSize, Vector(0, 0), scale);

	Vector numberPos = Vector(textPos.x + offsetX, textPos.y);
	Vector numSrcPos = Vector(135.0f + _currentCount * 64.0f, 0.0f);
	Vector numSize = Vector(64.0f, 64.0f);
	_smallSheetTexture->Render(renderTarget, numberPos, numSrcPos, numSize, Vector(0, 0), scale);


	// BigContinue
	Vector centerPos = Vector(GWinSizeX / 2.0f, GWinSizeY / 2.0f);
	Vector bigScale = Vector(1.5f, 1.5f); // 원하는 크기 배율

	Vector contSrcPos = Vector(BIG_CONTINUE_TEXT_RECT.left, BIG_CONTINUE_TEXT_RECT.top);
	Vector contSize = Vector(BIG_CONTINUE_TEXT_RECT.right - BIG_CONTINUE_TEXT_RECT.left,
		BIG_CONTINUE_TEXT_RECT.bottom - BIG_CONTINUE_TEXT_RECT.top);
	Vector contPos = Vector(centerPos.x - 70.0f * bigScale.x, centerPos.y);
	_bigSheetTexture->Render(renderTarget, contPos, contSrcPos, contSize, Vector(0, 0), bigScale);

	Vector qSrcPos = Vector(BIG_QUESTION_RECT.left, BIG_QUESTION_RECT.top);
	Vector qSize = Vector(BIG_QUESTION_RECT.right - BIG_QUESTION_RECT.left,
		BIG_QUESTION_RECT.bottom - BIG_QUESTION_RECT.top);
	Vector qPos = Vector(centerPos.x + 65.0f * bigScale.x, centerPos.y);

	_bigSheetTexture->Render(renderTarget, qPos, qSrcPos, qSize, Vector(0, 0), bigScale);

	if (_currentCount >= 0 && _currentCount <= 9)
	{
		const D2D1_RECT_F& numRect = BIG_NUMBER_RECTS[_currentCount];
		Vector numSrcPos = Vector(numRect.left, numRect.top);
		Vector numSize = Vector(numRect.right - numRect.left, numRect.bottom - numRect.top);
		Vector numPos = Vector(centerPos.x + 130.0f * bigScale.x, centerPos.y);

		_bigSheetTexture->Render(renderTarget, numPos, numSrcPos, numSize, Vector(0, 0), bigScale);
	}
}

void ContinueUI::StartCountdown(int startCount)
{
	_currentCount = startCount;
	_timer = 0.0f;
	_state = ContinueState::Counting;
	SetVisible(true); // 카운트다운 시작과 함께 UI 활성화
}
