#include "pch.h"
#include "Background.h"
#include "AnimatorComponent.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
#include "Texture.h"
void Background::Init()
{
	Super::Init();

	ImageRenderer* renderer = AddComponent<ImageRenderer>();
	Texture* tex = ResourceManager::GetInstance().GetTexture(L"Background");
	renderer->SetTexture(tex);
	if (tex)
	{
		// 2. 윈도우 해상도 / 텍스처 원본 크기로 배율(Scale) 자동 계산
		float scaleX = (float)GWinSizeX / (float)tex->GetSizeX();
		float scaleY = (float)GWinSizeY / (float)tex->GetSizeY();

		// 3. TransformComponent (또는 Actor)에 Scale 적용!
		SetScale(Vector(scaleX, scaleY));

		
		SetPos(Vector((float)GWinSizeX / 2.0f, (float)GWinSizeY/2.0f));
	}
	
}


void Background::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Background::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}
