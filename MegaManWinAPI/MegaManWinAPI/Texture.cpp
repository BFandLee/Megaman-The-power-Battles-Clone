#include "pch.h"
#include "Texture.h"
#include "Game.h"
#include "Scene.h"

void Texture::Load(wstring texturePath, int32 transparent, int32 row, int32 col, float dur, bool enableFlip)
{
	HDC hdc = ::GetDC(Game::GetInstance().GetHwnd());

	_bitmapHdc = ::CreateCompatibleDC(hdc);
	_bitmap = (HBITMAP)::LoadImageW(
		nullptr,
		texturePath.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	// 투명하게 보여야하는 색상값
	_transparent = transparent;

	if (_bitmap == 0)
	{
		::MessageBox(Game::GetInstance().GetHwnd(), texturePath.c_str(), L"Invalid Texture Load", MB_OK);
		return;
	}

	HBITMAP prev = (HBITMAP)::SelectObject(_bitmapHdc, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);

	_bitmapSizeX = bit.bmWidth;
	_bitmapSizeY = bit.bmHeight;

	// 행/열로 쪼개진 sprite 
	_col = col;
	_row = row;
	_dur = dur;

	// 쪼개진 1개의 frame size
	_frameSizeX = _bitmapSizeX / _col;
	_frameSizeY = _bitmapSizeY / _row;

	// 디폴트 값으로는 원본 비트맵의 크기로 설정
	_sizeX = _frameSizeX;
	_sizeY = _frameSizeY;

	// 좌우 반전된 텍스처가 필요하다면 미리 생성해둔다
	if (enableFlip && _bitmapHdc)
	{
		_flipBitmapHdc = ::CreateCompatibleDC(hdc);
		_flipBitmap = ::CreateCompatibleBitmap(hdc, _bitmapSizeX, _bitmapSizeY);
		::SelectObject(_flipBitmapHdc, _flipBitmap);
		::StretchBlt(_flipBitmapHdc,
			_bitmapSizeX - 1, 0, -(int32)_bitmapSizeX, _bitmapSizeY,
			_bitmapHdc, 0, 0, _bitmapSizeX, _bitmapSizeY,
			SRCCOPY);
	}
}

void Texture::Render(ID2D1HwndRenderTarget* renderTarget, Vector worldPos, Vector srcPos, bool flipX)
{
	// 가운데 좌표기준으로 그림이 그려지게 보정해주자.
	Vector renderPos = worldPos;
	
	if (_applyCenter)
	{
		renderPos.x -= (_sizeX * 0.5f);
		renderPos.y -= (_sizeY * 0.5f);
	}

	int32 srcX = (int32)srcPos.x;
	int32 srcY = (int32)srcPos.y;

	if (flipX && _flipBitmapHdc)
	{
		srcDC = _flipBitmapHdc;
		srcX = _bitmapSizeX - (int32)srcPos.x - _frameSizeX;
	}

}


void Texture::RenderScreen(ID2D1HwndRenderTarget* renderTarget, Vector screenPos, Vector srcPos)
{
	if (_applyCenter)
	{
		screenPos.x -= (_frameSizeX * 0.5f);
		screenPos.y -= (_frameSizeX * 0.5f);
	}

}
