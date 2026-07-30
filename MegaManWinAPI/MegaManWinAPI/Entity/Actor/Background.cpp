#include "pch.h"
#include "Background.h"
#include "AnimatorComponent.h"
#include "ResourceManager.h"
#include "ImageRenderer.h"
void Background::Init()
{
	Super::Init();

	_animator = AddComponent<AnimatorComponent>();
	

	AnimationClip* clip = new AnimationClip();
	clip->texture = ResourceManager::GetInstance().GetTexture(L"Background");

	AnimationFrame frame;
	frame.startPos = Vector(0, 0);
	frame.size = Vector(800, 400);
	frame.offset = Vector(0, 0);
	frame.duration = 0.1f;

	clip->frames.push_back(frame);
	clip->bLoop = true;

	ImageRenderer* renderer = AddComponent<ImageRenderer>();

	Texture* tex = ResourceManager::GetInstance().GetTexture(L"Background");
	renderer->SetTexture(tex);
}


void Background::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Background::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}
