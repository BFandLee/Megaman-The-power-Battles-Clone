#include "pch.h"
#include "GameClearScene.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Game.h"
#include "AnimatorComponent.h"
#include <dwrite.h>

GameClearScene::GameClearScene()
{
	_sceneType = SceneType::GameClear;
}

GameClearScene::~GameClearScene()
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

void GameClearScene::loadResources()
{
	ResourceManager::GetInstance().LoadAllTexturesInDirectory(L"../Resources/sprites");
}

void GameClearScene::Init()
{
	// Scene::Init()(Super::Init())을 의도적으로 호출하지 않아 SceneData.json이 로드되는 것을 방지합니다.
	loadResources();

	_gameClearTexture = ResourceManager::GetInstance().GetTexture(L"GameClear");

	_blinkTimer = 0.0f;
	_showSubText = true;

	// GameClear BGM 1회 재생 (루프 없음)
	SoundManager::GetInstance().PlayBGM(L"GameClear", false);

	// DirectWrite TextFormat 생성
	IDWriteFactory* dwriteFactory = Game::GetInstance().GetDWriteFactory();
	if (dwriteFactory)
	{
		// "Game Clear!" 대형 폰트 생성
		dwriteFactory->CreateTextFormat(
			L"맑은 고딕",
			nullptr,
			DWRITE_FONT_WEIGHT_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			48.0f,
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

	// 1. 중앙 록맨 승리 애니메이션 액터 생성 및 배치
	_victoryActor = new VictoryMegamanActor();
	_victoryActor->Init();
	_victoryActor->SetPos(Vector(GWinSizeX / 2.0f, (float)GWinSizeY * 0.62f));
	_victoryActor->SetScale(Vector(2.5f, 2.5f));

	AnimatorComponent* playerAnim = _victoryActor->AddComponent<AnimatorComponent>();
	if (playerAnim != nullptr)
	{
		playerAnim->LoadAnimationFromJson(L"Victory", L"../Resources/sprites/Player/Animation/Victory.json");
		playerAnim->Play(L"Victory");
	}

	AddActor(_victoryActor);

	// 2. 우하단 제미니맨 Idle 애니메이션 액터 생성 및 배치 (중앙을 바라보도록 X 반전)
	_geminiManActor = new BossIdleActor();
	_geminiManActor->Init();
	_geminiManActor->SetPos(Vector((float)GWinSizeX * 0.85f, (float)GWinSizeY * 0.65f));
	_geminiManActor->SetScale(Vector(-2.5f, 2.5f));

	AnimatorComponent* bossAnim = _geminiManActor->AddComponent<AnimatorComponent>();
	if (bossAnim != nullptr)
	{
		bossAnim->LoadAnimationFromJson(L"BossIdle", L"../Resources/sprites/Boss/Animation/Idle.json");
		bossAnim->Play(L"BossIdle");
	}

	AddActor(_geminiManActor);
}

void GameClearScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 하단 안내 텍스트 점멸 (0.5초 간격)
	_blinkTimer += deltaTime;
	if (_blinkTimer >= 0.5f)
	{
		_blinkTimer = 0.0f;
		_showSubText = !_showSubText;
	}

	// ENTER 키 또는 SpaceBar 입력 시 로비(LobbyScene)로 복귀
	if (InputManager::GetInstance().GetButtonDown(KeyType::ENTER) ||
		InputManager::GetInstance().GetButtonDown(KeyType::SpaceBar))
	{
		SceneManager::GetInstance().ChangeScene(SceneType::Lobby);
	}
}

void GameClearScene::Render(ID2D1RenderTarget* renderTarget)
{
	if (!renderTarget) return;

	// 1. 검은색 배경 클리어
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	// 2. GameClear 배경 이미지를 화면 전체에 꽉 차게 스트레칭하여 렌더링
	if (_gameClearTexture)
	{
		SIZE frameSize = _gameClearTexture->GetFrameSize();
		float scaleX = (frameSize.cx > 0) ? ((float)GWinSizeX / (float)frameSize.cx) : 1.0f;
		float scaleY = (frameSize.cy > 0) ? ((float)GWinSizeY / (float)frameSize.cy) : 1.0f;
		Vector centerPos = Vector((float)GWinSizeX * 0.5f, (float)GWinSizeY * 0.5f);
		_gameClearTexture->Render(renderTarget, centerPos, Vector(0.0f, 0.0f), Vector(scaleX, scaleY));
	}

	// 3. 중앙 "Game Clear!" 텍스트 렌더링
	if (_titleBrush == nullptr)
	{
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.85f, 0.0f), &_titleBrush);
	}

	if (_titleFormat && _titleBrush)
	{
		const WCHAR* titleText = L"Game Clear!";
		UINT32 titleLength = (UINT32)wcslen(titleText);
		D2D1_RECT_F titleRect = D2D1::RectF(
			0.0f,
			(float)GWinSizeY * 0.35f - 30.0f,
			(float)GWinSizeX,
			(float)GWinSizeY * 0.35f + 30.0f
		);

		renderTarget->DrawText(
			titleText,
			titleLength,
			_titleFormat,
			titleRect,
			_titleBrush
		);
	}

	// 4. 액터 렌더링 (중앙 승리 록맨 및 우하단 제미니맨 Idle)
	Super::Render(renderTarget);

	// 5. 하단 안내 텍스트 렌더링
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
			(float)GWinSizeY * 0.85f,
			(float)GWinSizeX,
			(float)GWinSizeY * 0.95f
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
