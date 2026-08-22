#pragma once
#include "Scene.h"

class Texture;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class LobbyScene : public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene() override;

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

protected:
	virtual void loadResources() override;

private:
	Texture* _bgTexture = nullptr;
	Texture* _titleTexture = nullptr;

	IDWriteTextFormat* _textFormat = nullptr;
	ID2D1SolidColorBrush* _textBrush = nullptr;
};
