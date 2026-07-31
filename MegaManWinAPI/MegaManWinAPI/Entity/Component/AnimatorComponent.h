#pragma once
#include "Component.h"

struct AnimationFrame
{
	Vector startPos;    // 텍스처 내 시작 좌표(좌상단)
	Vector size;        // 잘라낼 크기
	Vector offset;      // 중심축 조정용 오프셋
	float duration;     // 프레임 유지 시간
};

struct AnimationClip
{
	class Texture* texture = nullptr;
	vector<AnimationFrame> frames;
	bool bLoop = false;
};

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
	AnimatorComponent() {};
	virtual ~AnimatorComponent();

	// 게임 루프 갱신 및 그리기
	virtual void Update(float deletaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	// 클립 추가 및 재생 인터페이스
	void AddClip(const wstring& stateName, AnimationClip* clip);
	void Play(const wstring& stateName);

	// Json 파일을 읽어와 지정된 stateName으로 Clip을 등록하는 인터페이스
	bool LoadAnimationFromJson(const wstring& stateName, wstring& jsonFilePath);
};

