#include "pch.h"
#include "AnimatorComponent.h"
#include "Actor.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
AnimatorComponent::~AnimatorComponent()
{
}

void AnimatorComponent::Update(float deletaTime)
{
	Super::Update(deletaTime);

	// 방어 코드
	if (_currentClip == nullptr)
		return;

	// 시간 누적
	_accmulatedTime += deletaTime;

	float currentDuration = _currentClip->frames[_currentFrame].duration;

	if (_accmulatedTime >= currentDuration)
	{
		_accmulatedTime -= currentDuration;
		_currentFrame++;

		if (_currentFrame >= _currentClip->frames.size())
		{
			if (_currentClip->bLoop) 
				_currentFrame = 0;
			else
			{
				_currentFrame = (int32)_currentClip->frames.size() - 1;
			}
		}
	}
}

void AnimatorComponent::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);

	// 방어코드
	if (_currentClip == nullptr || _currentClip->texture == nullptr)
		return;

	// 현재 재생할 프레임 정보 꺼내기
	const AnimationFrame& frame = _currentClip->frames[_currentFrame];

	// Actor의 위치 가져오기
	Vector actorPos = GetOwner()->GetPos();
	Vector scale = GetOwner()->GetComponent<TransformComponent>()->GetScale();
	bool filpX = false;

	if (scale.x < 0)
	{
		filpX = true;
		scale.x = abs(scale.x);
	}
	_currentClip->texture->Render(renderTarget, actorPos, frame.startPos, frame.size, frame.offset, scale, filpX);
}

void AnimatorComponent::AddClip(const wstring& stateName, AnimationClip* clip)
{
}

void AnimatorComponent::Play(const wstring& stateName)
{
	// map에서 stateName 키가 존재하는지 찾기
	auto it = _clips.find(stateName);

	// 찾지 못했다면 함수 종료
	if (it == _clips.end())
	{
		return;
	}

	// 찾았다면 _currentClip을 교체하고, 프레임과 누적 시간 초기화
	_currentClip = it->second;
	_currentFrame = 0;
	_accmulatedTime = 0.0f;
}

bool AnimatorComponent::LoadAnimationFromJson(const wstring& stateName, const wstring& jsonFilePath)
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

	if (j.contains("bLoop"))
	{
		clip->bLoop = j["bLoop"];
	}

	clip->texture = ResourceManager::GetInstance().GetTexture(texKey);

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
