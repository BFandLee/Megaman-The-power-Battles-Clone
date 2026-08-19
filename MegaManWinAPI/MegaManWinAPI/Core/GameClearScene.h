#pragma once
#include "Scene.h"
#include "Actor.h"

class Texture;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class VictoryMegamanActor : public Actor
{
public:
	VictoryMegamanActor() : Actor("VictoryMegaman") {}
	virtual ~VictoryMegamanActor() override {}

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Player; }
	virtual ActorType GetActorType() override { return ActorType::Player; }
};

class BossIdleActor : public Actor
{
public:
	BossIdleActor() : Actor("BossIdleActor") {}
	virtual ~BossIdleActor() override {}

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Boss; }
	virtual ActorType GetActorType() override { return ActorType::Boss; }
};

class GameClearScene : public Scene
{
	using Super = Scene;
public:
	GameClearScene();
	virtual ~GameClearScene() override;

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

protected:
	virtual void loadResources() override;

private:
	Texture* _gameClearTexture = nullptr;

	IDWriteTextFormat* _titleFormat = nullptr;
	IDWriteTextFormat* _subFormat = nullptr;
	ID2D1SolidColorBrush* _titleBrush = nullptr;
	ID2D1SolidColorBrush* _subBrush = nullptr;

	float _blinkTimer = 0.0f;
	bool _showSubText = true;

	VictoryMegamanActor* _victoryActor = nullptr;
	BossIdleActor* _geminiManActor = nullptr;
};
