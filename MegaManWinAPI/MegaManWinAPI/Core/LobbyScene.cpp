#include "pch.h"
#include "LobbyScene.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Game.h"
#include <dwrite.h>

LobbyScene::LobbyScene()
{
	_sceneType = SceneType::Lobby;
}

LobbyScene::~LobbyScene()
{
	if (_textFormat)
	{
		_textFormat->Release();
		_textFormat = nullptr;
	}
	if (_textBrush)
	{
		_textBrush->Release();
		_textBrush = nullptr;
	}
}

void LobbyScene::loadResources()
{
	ResourceManager::GetInstance().LoadAllTexturesInDirectory(L"Resources/sprites");
}

void LobbyScene::Init()
{
	// 리소스 로드
	loadResources();

	// 배경 및 타이틀 텍스처 로드
	_bgTexture = ResourceManager::GetInstance().GetTexture(L"background");
	if (_bgTexture == nullptr)
	{
		_bgTexture = ResourceManager::GetInstance().GetTexture(L"Background");
	}

	_titleTexture = ResourceManager::GetInstance().GetTexture(L"Title");
	if (_titleTexture == nullptr)
	{
		_titleTexture = ResourceManager::GetInstance().GetTexture(L"title");
	}

	if (_bgTexture)
	{
		_bgTexture->SetApplyCenter(false);
	}

	// Title BGM 루프 재생
	SoundManager::GetInstance().PlayBGM(L"Title", true);

	// DirectWrite TextFormat 생성
	IDWriteFactory* dwriteFactory = Game::GetInstance().GetDWriteFactory();
	if (dwriteFactory)
	{
		dwriteFactory->CreateTextFormat(
			L"맑은 고딕",
			nullptr,
			DWRITE_FONT_WEIGHT_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			22.0f,
			L"ko-kr",
			&_textFormat
		);

		if (_textFormat)
		{
			_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}
}

void LobbyScene::Update(float deltaTime)
{
	Scene::Update(deltaTime);

	// ENTER 또는 SpaceBar 키 입력 시 게임(TestScene)으로 전환
	if (InputManager::GetInstance().GetButtonDown(KeyType::ENTER) ||
		InputManager::GetInstance().GetButtonDown(KeyType::SpaceBar))
	{
		SceneManager::GetInstance().ChangeScene(SceneType::Test);
	}
}

void LobbyScene::Render(ID2D1RenderTarget* renderTarget)
{
	if (!renderTarget) return;

	// 1. 배경 이미지 화면에 꽉 차게 렌더링
	if (_bgTexture)
	{
		float scaleX = (float)GWinSizeX / (float)_bgTexture->GetSizeX();
		float scaleY = (float)GWinSizeY / (float)_bgTexture->GetSizeY();
		_bgTexture->Render(renderTarget, Vector(0.0f, 0.0f), Vector(0.0f, 0.0f), Vector(scaleX, scaleY));
	}

	// 2. 타이틀 로고 중앙 렌더링
	if (_titleTexture)
	{
		float titleScale = 1.8f;
		Vector titlePos = Vector(GWinSizeX / 2.0f, (GWinSizeY / 2.0f) - 30.0f);
		_titleTexture->Render(renderTarget, titlePos, Vector(0.0f, 0.0f), Vector(titleScale, titleScale));
	}

	// 3. 안내 문구 텍스트 렌더링 ("Enter키를 눌러 시작하세요!")
	if (_textFormat)
	{
		if (_textBrush == nullptr)
		{
			renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &_textBrush);
		}

		if (_textBrush)
		{
			const WCHAR* text = L"Enter키를 눌러 시작하세요!";
			UINT32 textLength = (UINT32)wcslen(text);
			D2D1_RECT_F layoutRect = D2D1::RectF(
				0.0f,
				(float)GWinSizeY * 0.72f,
				(float)GWinSizeX,
				(float)GWinSizeY * 0.85f
			);

			renderTarget->DrawText(
				text,
				textLength,
				_textFormat,
				layoutRect,
				_textBrush
			);
		}
	}

	Scene::Render(renderTarget);
}
