#include "pch.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Actor.h"

void SpriteAnimRenderer::Init(wstring textureKey)
{
	_texture = ResourceManager::GetInstance().GetTexture(textureKey);
	_durtaion = _texture->GetDur();
}

void SpriteAnimRenderer::Update(float deltaTime)
{
	if (_isEnd)
		return;

	if (_texture == nullptr)
		return;

	if (_durtaion <= 0)
		return;

	_sumTime += deltaTime;

	int32 frameCountX = 0;
	int32 frameCountY = 0;
	_texture->GetFrameCount(frameCountX, frameCountY);

	int32 totalCount = _fullFrame ? (frameCountX * frameCountY) : frameCountX;
	if (totalCount <= 0)
		return;

	float frameTime = _durtaion / totalCount;

	// 일정 시간이 지나면 다음 프레임 이동
	if (_sumTime >= frameTime)
	{
		_sumTime -= frameTime;

		// 현재 인덱스를 선형적으로 계산 (fullFrame 여부에 따라 범위가 달라짐)
		int32 currentIndex = _fullFrame ? (_animIndexY * frameCountX + _animIndexX) : _animIndexX;
		int32 nextIndex = currentIndex + 1;

		if (nextIndex >= totalCount)
		{
			if (_loop)
			{
				nextIndex = 0;
			}
			else
			{
				_isEnd = true;

				// 함수 포인터 호출
				if (_callBack)
				{
					_callBack();
				}
				return; // 마지막 프레임 유지
			}
		}

		// 인덱스 업데이트
		_animIndexX = nextIndex % frameCountX;
		
		if (_fullFrame)
		{
			_animIndexY = nextIndex / frameCountX;
		}
	}
}

void SpriteAnimRenderer::Render(ID2D1RenderTarget* renderTarget)
{
	Vector pos = GetSpirtePos();
	if (nullptr == _texture)
		return;

	// 프레임 위치에 따라 잘라낼 시작 위치 계산(출발지 계산)
	Vector srcPos;
	srcPos.x = _animIndexX + _texture->GetFrameSize().cx;
	srcPos.y = _animIndexY + _texture->GetFrameSize().cy;

	_texture->Render(renderTarget, pos, srcPos);
}

uint32 SpriteAnimRenderer::GetSizeX() const
{
	if (_texture)
	{
		return _texture->GetSizeX();
	}

	return 0;
}

uint32 SpriteAnimRenderer::GetSizeY() const
{
	if (_texture)
	{
		return _texture->GetSizeY();
	}

	return 0;
}

void SpriteAnimRenderer::SetSize(int32 x, int32 y)
{
	if (_texture)
	{
		return _texture->SetSize(x, y);
	}
}

Vector SpriteAnimRenderer::GetSpirtePos()
{
	return GetOwner()->GetPos();
}

void SpriteAnimRenderer::ResetAnim(int32 row, bool loop, float duration)
{
	_animIndexX = 0;
	_animIndexY = row;
	_isEnd = false;
	_sumTime = 0;
	_loop = loop;
	_durtaion = duration;
}
