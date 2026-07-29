#pragma once
#include "Component.h"
struct AnimationClip;	// 플레이어 구현 시 자세히 구현될 클립 구조체

class AnimatorComponent : public Component
{
	using Super = Component;
private:
	// 플레이어 상태에 따른 애니메이션 클립 보관
	map<wstring, AnimationClip*> _clips;

	// 현재 재생 중인 애니메이션과 관련 데이터
	AnimationClip* _currentClip = nullptr;
	int32 _currentFrame = 0;
	float _accmulatedTime = 0.0f;

public:
	AnimatorComponent();
	virtual ~AnimatorComponent();

	// 게임 루프 갱신 및 그리기
	virtual void Update(float deletaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	// 클립 추가 및 재생 인터페이스
	void AddClip(const wstring& stateName, AnimationClip* clip);
	void Play(const wstring& stateName);
};

