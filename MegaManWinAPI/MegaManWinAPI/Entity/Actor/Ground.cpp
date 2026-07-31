#include "pch.h"
#include "Ground.h"
#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"

void Ground::Init()
{
	Super::Init();

	// TODO 1: ImageRenderer 컴포넌트를 추가하세요.
	ImageRenderer* renderer = AddComponent<ImageRenderer>();

	// TODO 2: ResourceManager를 통해 Ground 텍스처를 가져오고 렌더러에 세팅하세요.
	// (주의: TestScene의 loadResources()에 텍스처 로드 코드를 먼저 추가해야 합니다!)
	Texture* tex = ResourceManager::GetInstance().GetTexture(L"Ground");
	renderer->SetTexture(tex);

	// TODO 3: 화면 크기(GWinSizeX, GWinSizeY)와 텍스처 크기를 고려하여
	// 화면 하단에 꽉 차게 나오도록 Scale과 Pos를 설정하세요.
	// 힌트: Y좌표는 화면 최하단(GWinSizeY)에서 (이미지 원본 높이 * ScaleY / 2.0f)를 빼주는 방식으로 설정하면 바닥에 딱 맞습니다.
	
	if (tex)
	{
		float scaleX = (float)GWinSizeX / (float)tex->GetSizeX();
		float scaleY = scaleX;
		SetScale(Vector(scaleX, scaleY));

		float posX = (float)GWinSizeX / 2.0f;
		float posY = ((float)GWinSizeY + _weight) - ((float)tex->GetSizeY() * scaleY / 2.0f);
		SetPos(Vector(posX, posY));
	}
	
}

void Ground::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Ground::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}
