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

bool AnimatorComponent::LoadAnimationFromJson(const wstring& stateName, wstring& jsonFilePath)
{
	// 파일 스트림 열기
	std::ifstream file(jsonFilePath);
	if (!file.is_open())
		return false;

	// 2. nlohmann/json 객체로 파싱
	json j;
	file >> j;

	// 3. 데이터를 담을 빈 클립 생성
	AnimationClip* clip = new AnimationClip();

	// 텍스처 로드
	string texStr = j["texturePath"];
	wstring texKey;
	texKey.assign(texStr.begin(), texStr.end());

	// 프레임 파싱
	for (auto& frameJson : j["frames"])
	{
		AnimationFrame frame;

		frame.startPos.x = frameJson["startPos"]["x"];
		frame.startPos.y = frameJson["startPos"]["y"];
		frame.size.x = frameJson["size"]["x"];
		frame.size.y = frameJson["size"]["y"];
		frame.offset.x = frameJson["offset"]["x"];
		frame.offset.y = frameJson["offset"]["y"];
		frame.duration = frameJson["duration"];

		clip->frames.push_back(frame);

	}

	// 4. 완성된 클립을 _clips 맵에 등록
	_clips[stateName] = clip;
	return true;
}
