#include "pch.h"
#include "AnimatorComponent.h"
#include "Actor.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "InputManager.h"



AnimatorComponent::~AnimatorComponent()
{
}

void AnimatorComponent::Update(float deletaTime)
{
	Super::Update(deletaTime);

	// 방어 코드
	if (_currentClip == nullptr)
		return;

	if (InputManager::GetInstance().GetButtonDown(KeyType::F1))
	{
		_bIsEditMode = !_bIsEditMode;
	}

	if (_bIsEditMode)
	{
		UpdateFrameIndex();
		UpdateFrameOffset();

		SaveToJson();
	}

	else
	{
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
					_currentFrame = _currentClip->loopStartIndex;
				else
				{
					_currentFrame = (int32)_currentClip->frames.size() - 1;
				}
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

void AnimatorComponent::UpdateFrameIndex()
{
	if (InputManager::GetInstance().GetButtonDown(KeyType::LeftBracket))
	{
		_currentFrame--;
		if (_currentFrame < 0)
		{
			_currentFrame = _currentClip->frames.size() - 1;
		}
	}
	else if (InputManager::GetInstance().GetButtonDown(KeyType::RightBracket))
	{
		_currentFrame++;
		if (_currentFrame >= _currentClip->frames.size())
		{
			_currentFrame = 0;
		}
	}
}

void AnimatorComponent::UpdateFrameOffset()
{
	if (InputManager::GetInstance().GetButtonPressed(KeyType::Left))
	{
		_currentClip->frames[_currentFrame].offset.x--;
	}
	else if (InputManager::GetInstance().GetButtonPressed(KeyType::Right))
	{
		_currentClip->frames[_currentFrame].offset.x++;
	}

	if (InputManager::GetInstance().GetButtonPressed(KeyType::Up))
	{
		_currentClip->frames[_currentFrame].offset.y--;
	}
	else if (InputManager::GetInstance().GetButtonPressed(KeyType::Down))
	{
		_currentClip->frames[_currentFrame].offset.y++;
	}
}

void AnimatorComponent::SaveToJson()
{
	if (InputManager::GetInstance().GetButtonDown(KeyType::LeftCtrl))
	{

	}
}

void AnimatorComponent::RenderUI()
{

	// 1. ImGui 창 시작
	ImGui::Begin("Animation Offset Editor");
	if (_currentClip != nullptr && _currentClip->frames.size() > 0)
	{
		// 2. 현재 프레임 정보 가져오기
		AnimationFrame& currentFrameData = _currentClip->frames[_currentFrame];

		// 3. 슬라이더 만들기 (드래그로 x, y 실시간 조절)
		ImGui::Text("Current Frame: %d", _currentFrame);
		ImGui::DragFloat("Offset X", &currentFrameData.offset.x, 1.0f);
		ImGui::DragFloat("Offset Y", &currentFrameData.offset.y, 1.0f);

		// 4. 저장 버튼 만들기
		if (ImGui::Button("Save to JSON"))
		{
			// TODO: json 객체를 하나 만들고, _currentClip->frames 데이터를 반복문으로 넣기
			json frameJson;
			// (힌트: 아까 LoadAnimationFromJson에서 읽어오던 방식을 반대로 쓰면 됩니다!)
			for (auto& Clipframe : _currentClip->frames)
			{
				AnimationFrame frame = Clipframe;

				frameJson["startPos"]["x"] = frame.startPos.x;
				frameJson["startPos"]["y"]; frame.startPos.y;
				frameJson["size"]["x"] = frame.size.x;
				frameJson["size"]["y"] = frame.size.y;
				frameJson["offset"]["x"] = frame.offset.x;
				frameJson["offset"]["y"] = frame.offset.y;
				frameJson["duration"] = frame.duration;
			}
			// TODO: std::ofstream을 열어서 특정 파일경로(예: Charge.json)에 j 덮어쓰기

			std::ofstream("D:\\Megaman-The-power-Battles-Clone\\MegaManWinAPI\\Resources\\sprites\\AttackEffect\\Animation\\Charge.json");
		}
	}
	ImGui::End();
}

void AnimatorComponent::AddClip(const wstring& stateName, AnimationClip* clip)
{
}

void AnimatorComponent::Play(const wstring& stateName, bool keepFrame)
{
	// map에서 stateName 키가 존재하는지 찾기
	auto it = _clips.find(stateName);

	// 찾지 못했다면 함수 종료
	if (it == _clips.end())
	{
		return;
	}

	// 찾았다면 _currentClip을 교체하고, 프레임과 누적 시간 초기화
	if (_currentClip == it->second)
	{
		return;
	}
	_currentClip = it->second;

	if (!keepFrame)
	{
		_currentFrame = 0;
		_accmulatedTime = 0.0f;
	}
	else
	{
		// Exception Index Gude
		if (_currentClip->frames.size() > 0 && _currentFrame >= _currentClip->frames.size())
		{
			_currentFrame = _currentClip->frames.size() - 1;
		}
	}
	
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
	clip->sourceFilePath = jsonFilePath;

	// 텍스처 로드
	string texStr = j["texturePath"];
	wstring texKey;
	texKey.assign(texStr.begin(), texStr.end());

	if (j.contains("bLoop"))
	{
		clip->bLoop = j["bLoop"];
	}

	if (j.contains("loopStartIndex"))
	{
		clip->loopStartIndex = j["loopStartIndex"];
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
