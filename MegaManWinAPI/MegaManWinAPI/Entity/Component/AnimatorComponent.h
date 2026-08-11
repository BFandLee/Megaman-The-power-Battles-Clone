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
	class Texture* swapTexture = nullptr;
	vector<AnimationFrame> frames;
	bool bLoop = false;
	int32 loopStartIndex = 0;
	wstring sourceFilePath;
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

	void UpdateFrameIndex();
	void UpdateFrameOffset();
	void SaveToJson();

	// 애니메이션을 키로 하여 종료 시 호출할 콜백을 저장하는 맵
	unordered_map<wstring, std::function<void()>> _endEvents;

	// 이벤트 호출 시 현재 재생중인 애니메이션의 이름
	wstring _currentClipName = L"";

	bool _bisFinished = false;

	// 텍스처 색상 스왑 플래그 및 교체 텍스처 포인터
	bool _isYellowColor = false;
public:
	void SetTextureColor(bool textureColor) { _isYellowColor = textureColor; }
	void SetSwapTextureForState(const wstring& stateName, class Texture* swapTex);
	AnimatorComponent() : Component("AnimatorComponent") {}
	virtual ~AnimatorComponent();

	virtual void Update(float deletaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void RenderUI() override;

	// 클립 추가 및 재생 인터페이스
	void AddClip(const wstring& stateName, AnimationClip* clip);
	void Play(const wstring& stateName, bool keepFrame = false);

	// Json 파일을 읽어와 지정된 stateName으로 Clip을 등록하는 인터페이스
	bool LoadAnimationFromJson(const wstring& stateName, const wstring& jsonFilePath);
	bool _bIsEditMode = false;

	void ResetFrame();

	void SetEndEvent(const wstring& stateName, std::function<void()> callback)
	{
		_endEvents[stateName] = callback;
	}

};

