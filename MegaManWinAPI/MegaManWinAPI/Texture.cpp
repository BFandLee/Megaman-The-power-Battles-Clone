#include "pch.h"
#include "Texture.h"
#include "Game.h"
#include "Scene.h"

void Texture::Load(wstring texturePath, int32 row, int32 col, float dur, bool enableFlip)
{
	// 1. 만들어둔 팩토리와 renderTarget 가져오기
	IWICImagingFactory* wicFactory = Game::GetInstance().GetWICFactory();
	ID2D1HwndRenderTarget* renderTarget = Game::GetInstance().GetRenderTarget();

	if (!wicFactory || !renderTarget) return;

	// 2. 디코더 생성(이미지 파일 열기)
	IWICBitmapDecoder* decoder = nullptr;
	wicFactory->CreateDecoderFromFilename(
		texturePath.c_str(), nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder
	);

	// 3. 1번째 프레임 가져오기( 일반 이미지: 프레임 1개)
	IWICBitmapFrameDecode* frame = nullptr;
	decoder->GetFrame(0, &frame);

	// 4. 32비트 초기화
	IWICFormatConverter* converter = nullptr;
	wicFactory->CreateFormatConverter(&converter);

	converter->Initialize(
		frame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr, 0.0f, WICBitmapPaletteTypeCustom
	);

	// 5. Direct2D 전용 비트맵 생성
	renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &_bitmap);

	_bitmapSizeX = (uint32)_bitmap->GetSize().width;
	_bitmapSizeY = (uint32)_bitmap->GetSize().height;

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

	// 7. 메모리 해제
	converter->Release();
	frame->Release();
	decoder->Release();
}

void Texture::Render(ID2D1RenderTarget* renderTarget, Vector worldPos, Vector srcPos, bool flipX)
{
	// 가운데 좌표기준으로 그림이 그려지게 보정해주자.
	Vector renderPos = worldPos;
	
	float left = worldPos.x;
	float top = worldPos.y;
	if (_applyCenter)
	{
		left -= (_sizeX * 0.5f);
		top -= (_sizeY * 0.5f);
	}

	// 도착지
	D2D1_RECT_F destRect = D2D1::RectF(left, top, left + _sizeX, top + _sizeY);

	// 출발지(원본 스프라이트 시트에서 잘라낼 사각형 영역)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcPos.x,
		srcPos.y,
		srcPos.x + _frameSizeX,
		srcPos.y + _frameSizeY
	);

	// 좌우 반전
	if (flipX)
	{
		// worldPos을 기준으로 x축 배율을 -1로 곱해 거울 반전을 만듦
		D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f),
			D2D1::Point2F(worldPos.x, worldPos.y)
		);
		renderTarget->SetTransform(scaleMatrix);
	}

	// 최종 그리기
	renderTarget->DrawBitmap(
		_bitmap,
		destRect,	// 도착지
		1.0f,	   // 투명도
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect	   // 출발지
	);

	// 좌우반전 했다면 원상복구
	if (flipX)
	{
		renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	int32 srcX = (int32)srcPos.x;
	int32 srcY = (int32)srcPos.y;

	

}


void Texture::RenderScreen(ID2D1RenderTarget* renderTarget, Vector screenPos, Vector srcPos)
{
	if (_applyCenter)
	{
		screenPos.x -= (_frameSizeX * 0.5f);
		screenPos.y -= (_frameSizeX * 0.5f);
	}

}
