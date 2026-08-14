#include "pch.h"
#include "AnimatorComponent.h"
#include "Actor.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "InputManager.h"



void AnimatorComponent::SetSwapTextureForState(const wstring& stateName, Texture* swapTex)
{
	if (_clips.find(stateName) != _clips.end())
	{
		_clips[stateName]->swapTexture = swapTex;
	}
}

AnimatorComponent::~AnimatorComponent()
{
}

void AnimatorComponent::Update(float deletaTime)
{
	Super::Update(deletaTime);

	// 방어 코드
	if (_currentClip == nullptr)
		return;
	
	// 플래그로 애니메이션이 끝났는지 체크
	if (_bisFinished) return;

	// 시간 누적
	_accmulatedTime += deletaTime;
	float currentDuration = _currentClip->frames[_currentFrame].duration;

	if (_accmulatedTime >= currentDuration)
	{
		if (_currentFrame == (int32)_currentClip->frames.size() - 1)
		{
			auto it = _endEvents.find(_currentClipName);

			AnimationClip* prevClip = _currentClip;
			if (it != _endEvents.end())
			{
				it->second();
			}

			if (prevClip != _currentClip || !prevClip->bLoop)
			{
				return;
			}
		}

		if (_bIsEditMode)
			return;

		// 프레임 넘기기
		_accmulatedTime -= currentDuration;
		_currentFrame++;

		if (_currentFrame >= _currentClip->frames.size())
		{
			if (_currentClip->bLoop)
				_currentFrame = _currentClip->loopStartIndex;
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

	// TODO: 노란색 스왑 상태(플래그)가 켜져 있다면, _currentClip->texture 대신 교체용 텍스처를 렌더링하도록 조건부 처리를 작성하세요.
	if (_isYellowColor && _currentClip->swapTexture != nullptr)
	{
		_currentClip->swapTexture->Render(renderTarget, actorPos, frame.startPos, frame.size, frame.offset, scale, filpX);
	}
	else
	{
		_currentClip->texture->Render(renderTarget, actorPos, frame.startPos, frame.size, frame.offset, scale, filpX);
	}
}





void AnimatorComponent::RenderUI()
{
	// 1. ImGui 창 시작
	ImGui::PushID(this);
	ImGui::Text("[ Animtor ]");
	if (_currentClip != nullptr && _currentClip->frames.size() > 0)
	{
		// 2. 현재 프레임 정보 가져오기
		AnimationFrame& currentFrameData = _currentClip->frames[_currentFrame];

		// 3. 슬라이더 만들기 (드래그로 x, y 실시간 조절)
		ImGui::Text("Current Frame: %d", _currentFrame);
		ImGui::DragFloat("Offset X", &currentFrameData.offset.x, 1.0f);
		ImGui::DragFloat("Offset Y", &currentFrameData.offset.y, 1.0f);

		ImGui::Checkbox("Edit Mode", &_bIsEditMode);

		if (_bIsEditMode)
		{
			UpdateFrameIndex();
			UpdateFrameOffset();

			string currentName(_currentClipName.begin(), _currentClipName.end());

			if (ImGui::BeginCombo("select Clip", currentName.c_str()))
			{
				for (auto& pair : _clips)
				{
					string clipName(pair.first.begin(), pair.first.end());
					bool isSelected = (_currentClipName == pair.first);

					if (ImGui::Selectable(clipName.c_str(), isSelected))
					{
						_currentClip = pair.second;
						_currentClipName = pair.first;
						_currentFrame = 0;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			
		}

		if (ImGui::Button("Save Json"))
		{
			SaveToJson();
		}
	}
	ImGui::PopID();
}

void AnimatorComponent::AddClip(const wstring& stateName, AnimationClip* clip)
{
}

void AnimatorComponent::Play(const wstring& stateName, bool keepFrame)
{
	if (_bIsEditMode == true)
		return;
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
	_currentClipName = stateName;
	_bisFinished = false;

	if (!keepFrame)
	{
		_currentFrame = 0;
		_accmulatedTime = 0.0f;
	}

	// Exception Index Gude
	if (_currentClip->frames.size() > 0 && _currentFrame >= _currentClip->frames.size())
	{
		_currentFrame = _currentClip->frames.size() - 1;
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

void AnimatorComponent::ResetFrame()
{
	_currentFrame = 0;
	_accmulatedTime = 0.0f;
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
	std::ifstream file(_currentClip->sourceFilePath);
	if (!file.is_open())
		return;
	json j;
	file >> j;
	j["frames"].clear();

	for (auto& Clipframe : _currentClip->frames)
	{
		json frameJson;

		frameJson["startPos"]["x"] = Clipframe.startPos.x;
		frameJson["startPos"]["y"] = Clipframe.startPos.y;
		frameJson["size"]["x"] = Clipframe.size.x;
		frameJson["size"]["y"] = Clipframe.size.y;
		frameJson["offset"]["x"] = Clipframe.offset.x;
		frameJson["offset"]["y"] = Clipframe.offset.y;
		frameJson["duration"] = Clipframe.duration;

		j["frames"].push_back(frameJson);
	}
	std::ofstream outFile(_currentClip->sourceFilePath);
	if (outFile.is_open())
	{
		// dump(4) : 4칸 들여쓰기해서 텍스트로 만듦
		outFile << j.dump(4);
		outFile.close();
	}
}