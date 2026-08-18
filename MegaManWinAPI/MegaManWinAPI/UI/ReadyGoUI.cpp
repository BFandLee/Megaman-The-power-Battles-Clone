#include "pch.h"
#include "ReadyGoUI.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Player.h"

ReadyGoUI::ReadyGoUI()
{
	
}

void ReadyGoUI::Init()
{
	_readyTexture = ResourceManager::GetInstance().GetTexture(L"UI_ReadyGo_Sheet");
	SetPos(Vector(GWinSizeX / 2.0f, GWinSizeY / 2.0f));
}

void ReadyGoUI::Update(float deltaTime)
{
	if (_state == ReadyGoState::Ready)
	{
		_timer += deltaTime;
		_rotationAngle += 100.0f * deltaTime;

		if (_timer >= 1.5f)
		{
			_rotationAngle = 0.0f;
			_timer = 0.0f;
			_state = ReadyGoState::Go;
		}
	}

	if (_state == ReadyGoState::Go)
	{
		_timer += deltaTime;
		if (_timer >= 0.8f)
		{
			_state = ReadyGoState::Finished;
			if (_onFinished) _onFinished();
		}
	}
}

void ReadyGoUI::Render(ID2D1RenderTarget* renderTarget)
{
	if (_state == ReadyGoState::Finished || _readyTexture == nullptr) return;

	Vector pos = GetPos();
	Vector srcPos = (_state == ReadyGoState::Ready) ? Vector(0.0f, 0.0f) : Vector(108.0f, 0.0f);
	Vector size = Vector(108.0f, 64.0f);
	Vector offset = Vector(0.0f, 0.0f);

	float baseScale = 2.0f;
	Vector scale = Vector(baseScale, 2.0f);

	if (_state == ReadyGoState::Ready)
	{
		float flipScaleX = cosf(_timer * 3.141592f * 4.0f);
		scale.x *= flipScaleX;
	}
	float rotation = 0.0f;

	_readyTexture->Render(renderTarget, pos, srcPos, size, offset, scale, false, rotation);
}
