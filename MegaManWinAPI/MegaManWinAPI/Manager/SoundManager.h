#pragma once
#include "Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
	SoundManager() = default;
	~SoundManager();

public:
	void Init(HWND hwnd);
	void Cleanup();

	// BGM 제어 (.mp3 지원)
	void PlayBGM(const wstring& key, bool loop = true);
	void StopBGM();
	void PauseBGM();
	void ResumeBGM();

	// SFX 제어 (.wav 지원)
	void PlaySFX(const wstring& key, bool loop = false);
	void StopSFX(const wstring& key);

	LPDIRECTSOUND GetSoundDevice() { return _soundDevice; }

private:
	LPDIRECTSOUND _soundDevice = nullptr;
	wstring _currentBgmKey = L"";
	wstring _currentBgmAlias = L"current_bgm";
};