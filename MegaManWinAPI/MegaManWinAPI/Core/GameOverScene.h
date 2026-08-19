#pragma once
#include "Scene.h"

class Texture;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class GameOverScene : public Scene
{
public:
	GameOverScene();
	virtual ~GameOverScene() override;

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

protected:
	virtual void loadResources() override;

private:
	Texture* _gameOverTexture = nullptr;

	IDWriteTextFormat* _titleFormat = nullptr;
	IDWriteTextFormat* _subFormat = nullptr;
	ID2D1SolidColorBrush* _titleBrush = nullptr;
	ID2D1SolidColorBrush* _subBrush = nullptr;

	// 스크롤(슬라이드 업) 연출용 변수
	float _scrollTimer = 0.0f;
	const float _scrollDuration = 1.5f;

	float _blinkTimer = 0.0f;
	bool _showSubText = true;
};