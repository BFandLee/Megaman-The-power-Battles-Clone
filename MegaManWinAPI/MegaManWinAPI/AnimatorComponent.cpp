#include "pch.h"
#include "AnimatorComponent.h"

AnimatorComponent::~AnimatorComponent()
{
}

void AnimatorComponent::Update(float deletaTime)
{
	Super::Update(deletaTime);
}

void AnimatorComponent::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}

void AnimatorComponent::AddClip(const wstring& stateName, AnimationClip* clip)
{
}

void AnimatorComponent::Play(const wstring& stateName)
{
}
