#include "pch.h"
#include "GameOverScene.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Game.h"
#include <dwrite.h>
#include <cmath>

GameOverScene::GameOverScene()
{
	_sceneType = SceneType::GameOver;
}

GameOverScene::~GameOverScene()
{
	if (_titleFormat)
	{
		_titleFormat->Release();
		_titleFormat = nullptr;
	}
	if (_subFormat)
	{
		_subFormat->Release();
		_subFormat = nullptr;
	}
	if (_titleBrush)
	{
		_titleBrush->Release();
		_titleBrush = nullptr;
	}
	if (_subBrush)
	{
		_subBrush->Release();
		_subBrush = nullptr;
	}
}

void GameOverScene::loadResources()
{
	ResourceManager::GetInstance().LoadAllTexturesInDirectory(L"Resources/sprites");
}

void GameOverScene::Init()
{
	loadResources();

	_gameOverTexture = ResourceManager::GetInstance().GetTexture(L"GameOver");

	_scrollTimer = 0.0f;
	_blinkTimer = 0.0f;
	_showSubText = true;

	// GameOver BGM 1회 재생 (루프 없음)
	SoundManager::GetInstance().PlayBGM(L"Game Over", false);

	// DirectWrite TextFormat 생성
	IDWriteFactory* dwriteFactory = Game::GetInstance().GetDWriteFactory();
	if (dwriteFactory)
	{
		// 굵고 큰 "GAME OVER" 폰트 생성 (텍스처 미사용 시 대비)
		dwriteFactory->CreateTextFormat(
			L"맑은 고딕",
			nullptr,
			DWRITE_FONT_WEIGHT_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			56.0f,
			L"ko-kr",
			&_titleFormat
		);

		if (_titleFormat)
		{
			_titleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			_titleFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}

		// 안내 문구 폰트 생성
		dwriteFactory->CreateTextFormat(
			L"맑은 고딕",
			nullptr,
			DWRITE_FONT_WEIGHT_MEDIUM,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			22.0f,
			L"ko-kr",
			&_subFormat
		);

		if (_subFormat)
		{
			_subFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			_subFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}
}

void GameOverScene::Update(float deltaTime)
{
	Scene::Update(deltaTime);

	// 아래 -> 위 슬라이드 업 타이머 갱신
	if (_scrollTimer < _scrollDuration)
	{
		_scrollTimer += deltaTime;
		if (_scrollTimer >= _scrollDuration)
		{
			_scrollTimer = _scrollDuration;
		}
	}
	else
	{
		// 스크롤이 완료된 후 안내 텍스트 점멸 시작 (0.5초 간격)
		_blinkTimer += deltaTime;
		if (_blinkTimer >= 0.5f)
		{
			_blinkTimer = 0.0f;
			_showSubText = !_showSubText;
		}
	}

	// ENTER 키 또는 SpaceBar 입력 처리
	if (InputManager::GetInstance().GetButtonDown(KeyType::ENTER) ||
		InputManager::GetInstance().GetButtonDown(KeyType::SpaceBar))
	{
		// 스크롤 중 입력 시 즉시 중앙 안착 완료
		if (_scrollTimer < _scrollDuration)
		{
			_scrollTimer = _scrollDuration;
		}
		else
		{
			// 스크롤 완료 상태에서 입력 시 타이틀(LobbyScene)로 복귀
			SceneManager::GetInstance().ChangeScene(SceneType::Lobby);
		}
	}
}

void GameOverScene::Render(ID2D1RenderTarget* renderTarget)
{
	if (!renderTarget) return;

	// 1. 순수 검은색 배경으로 화면 클리어
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	// 2. Ease-Out Cubic 슬라이드 업 계산 (아래 -> 중앙 부드럽게 감속)
	float t = _scrollTimer / _scrollDuration;
	float easeOutProgress = 1.0f - std::pow(1.0f - t, 3.0f);

	float startPosY = (float)GWinSizeY + 200.0f;
	float targetPosY = (GWinSizeY / 2.0f) - 30.0f;
	float currentPosY = startPosY + (targetPosY - startPosY) * easeOutProgress;

	// 3. GameOver 스프라이트 또는 타이틀 텍스트 렌더링
	if (_gameOverTexture)
	{
		float titleScale = 1.8f;
		Vector titlePos = Vector(GWinSizeX / 2.0f, currentPosY);
		_gameOverTexture->Render(renderTarget, titlePos, Vector(0.0f, 0.0f), Vector(titleScale, titleScale));
	}
	else
	{
		if (_titleBrush == nullptr)
		{
			renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &_titleBrush);
		}

		if (_titleFormat && _titleBrush)
		{
			const WCHAR* titleText = L"GAME OVER";
			UINT32 titleLength = (UINT32)wcslen(titleText);
			D2D1_RECT_F titleRect = D2D1::RectF(
				0.0f,
				currentPosY - 40.0f,
				(float)GWinSizeX,
				currentPosY + 40.0f
			);

			renderTarget->DrawText(
				titleText,
				titleLength,
				_titleFormat,
				titleRect,
				_titleBrush
			);
		}
	}

	// 4. 스크롤 완료 후 "PRESS ENTER TO RETURN TO TITLE" 깜빡이는 안내 텍스트 렌더링
	if (_scrollTimer >= _scrollDuration)
	{
		if (_subBrush == nullptr)
		{
			renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &_subBrush);
		}

		if (_subFormat && _subBrush && _showSubText)
		{
			const WCHAR* subText = L"PRESS ENTER TO RETURN TO TITLE";
			UINT32 subLength = (UINT32)wcslen(subText);
			D2D1_RECT_F subRect = D2D1::RectF(
				0.0f,
				(float)GWinSizeY * 0.72f,
				(float)GWinSizeX,
				(float)GWinSizeY * 0.85f
			);

			renderTarget->DrawText(
				subText,
				subLength,
				_subFormat,
				subRect,
				_subBrush
			);
		}
	}
}