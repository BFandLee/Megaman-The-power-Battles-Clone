#include "pch.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "Sound.h"

SoundManager::~SoundManager()
{
	Cleanup();
}

void SoundManager::Init(HWND hwnd)
{
	// DirectSound 디바이스 생성
	if (FAILED(::DirectSoundCreate(NULL, &_soundDevice, NULL)))
	{
		::MessageBox(NULL, L"DirectSound Create Failed", L"SYSTEM ERROR", MB_OK);
		return;
	}

	// 우선순위 협조 레벨 설정
	if (FAILED(_soundDevice->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
	{
		::MessageBox(NULL, L"SetCooperativeLevel Failed", L"SYSTEM ERROR", MB_OK);
		return;
	}
}

void SoundManager::Cleanup()
{
	StopBGM();

	if (_soundDevice)
	{
		_soundDevice->Release();
		_soundDevice = nullptr;
	}
}

void SoundManager::PlayBGM(const wstring& key, bool loop)
{
	wstring bgmPath = ResourceManager::GetInstance().GetBGMPath(key);
	if (bgmPath.empty())
		return;

	// 기존 재생 중인 BGM 정지 및 닫기
	StopBGM();

	_currentBgmKey = key;

	// MCI 명령어로 MP3 파일 열기 및 재생
	wstring openCmd = std::format(L"open \"{}\" type mpegvideo alias {}", bgmPath, _currentBgmAlias);
	::mciSendString(openCmd.c_str(), NULL, 0, NULL);

	wstring playCmd = loop
		? std::format(L"play {} repeat", _currentBgmAlias)
		: std::format(L"play {}", _currentBgmAlias);
	::mciSendString(playCmd.c_str(), NULL, 0, NULL);
}

void SoundManager::StopBGM()
{
	if (!_currentBgmKey.empty())
	{
		wstring stopCmd = std::format(L"stop {}", _currentBgmAlias);
		wstring closeCmd = std::format(L"close {}", _currentBgmAlias);
		::mciSendString(stopCmd.c_str(), NULL, 0, NULL);
		::mciSendString(closeCmd.c_str(), NULL, 0, NULL);
		_currentBgmKey = L"";
	}
}

void SoundManager::PauseBGM()
{
	if (!_currentBgmKey.empty())
	{
		wstring pauseCmd = std::format(L"pause {}", _currentBgmAlias);
		::mciSendString(pauseCmd.c_str(), NULL, 0, NULL);
	}
}

void SoundManager::ResumeBGM()
{
	if (!_currentBgmKey.empty())
	{
		wstring resumeCmd = std::format(L"resume {}", _currentBgmAlias);
		::mciSendString(resumeCmd.c_str(), NULL, 0, NULL);
	}
}

void SoundManager::PlaySFX(const wstring& key, bool loop)
{
	Sound* sound = ResourceManager::GetInstance().GetSound(key);
	if (sound == nullptr)
		return;

	sound->Play(loop);
}

void SoundManager::StopSFX(const wstring& key)
{
	Sound* sound = ResourceManager::GetInstance().GetSound(key);
	if (sound == nullptr)
		return;

	sound->Stop(true);
}