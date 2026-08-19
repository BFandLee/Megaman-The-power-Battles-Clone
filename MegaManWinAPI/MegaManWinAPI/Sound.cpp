#include "pch.h"
#include "Sound.h"
#include "SoundManager.h"

Sound::Sound()
{
}

Sound::~Sound()
{
	if (_soundBuffer)
	{
		_soundBuffer->Release();
		_soundBuffer = nullptr;
	}
}

bool Sound::LoadWave(fs::path fullPath)
{
	if (fullPath.extension() != L".wav")
		return false;

	HMMIO file = ::mmioOpen((WCHAR*)fullPath.c_str(), NULL, MMIO_READ);
	if (file == 0)
		return false;

	MMCKINFO parent;
	::memset(&parent, 0, sizeof(parent));
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	::mmioDescend(file, &parent, NULL, MMIO_FINDRIFF);

	MMCKINFO child;
	::memset(&child, 0, sizeof(child));
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	::mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	WAVEFORMATEX wft;
	::memset(&wft, 0, sizeof(wft));
	::mmioRead(file, (char*)&wft, sizeof(wft));

	::mmioAscend(file, &child, 0);
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	::mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	::memset(&_bufferDesc, 0, sizeof(DSBUFFERDESC));
	_bufferDesc.dwBufferBytes = child.cksize;
	_bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	_bufferDesc.dwFlags = DSBCAPS_STATIC;
	_bufferDesc.lpwfxFormat = &wft;

	if (FAILED(SoundManager::GetInstance().GetSoundDevice()->CreateSoundBuffer(&_bufferDesc, &_soundBuffer, NULL)))
	{
		::mmioClose(file, 0);
		return false;
	}

	void* write1 = NULL;
	void* write2 = NULL;
	DWORD len1;
	DWORD len2;

	_soundBuffer->Lock(0, child.cksize, &write1, &len1, &write2, &len2, 0);
	if (write1)
		::mmioRead(file, (char*)write1, len1);
	if (write2)
		::mmioRead(file, (char*)write2, len2);

	_soundBuffer->Unlock(write1, len1, write2, len2);
	::mmioClose(file, 0);
	return true;
}

void Sound::Play(bool loop)
{
	if (_soundBuffer == nullptr)
		return;

	_soundBuffer->SetCurrentPosition(0);
	_soundBuffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
}

void Sound::Stop(bool reset)
{
	if (_soundBuffer == nullptr)
		return;

	_soundBuffer->Stop();
	if (reset)
		_soundBuffer->SetCurrentPosition(0);
}