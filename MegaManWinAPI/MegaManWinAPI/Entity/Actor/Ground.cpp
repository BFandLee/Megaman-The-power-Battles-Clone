#include "pch.h"
#include "Ground.h"
#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "BoxCollider.h"

void Ground::Init()
{
	Super::Init();

	ImageRenderer* renderer = AddComponent<ImageRenderer>();
	Texture* tex = ResourceManager::GetInstance().GetTexture(L"Ground");
	renderer->SetTexture(tex);
	
	if (tex)
	{
		float scaleX = (float)GWinSizeX / (float)tex->GetSizeX();
		float scaleY = scaleX;
		SetScale(Vector(scaleX, scaleY));

		float posX = (float)GWinSizeX / 2.0f;
		float posY = ((float)GWinSizeY + _weight) - ((float)tex->GetSizeY() * scaleY / 2.0f);
		SetPos(Vector(posX, posY));
	}

	BoxCollider* box = AddComponent<BoxCollider>();
	box->SetSize((float)GWinSizeX, (float)GWinSizeY/10.0f);
	box->SetOffset(Vector(0.0f, 2.0f));
	
}

void Ground::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Ground::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}
