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
	HRESULT hr = wicFactory->CreateDecoderFromFilename(
		texturePath.c_str(), nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder
	);
	// [방어 코드] 디코더 생성에 실패했거나 decoder가 nullptr이면 안전하게 중단!
	if (FAILED(hr) || !decoder)
	{
		// 디버그 출력 창에 실패한 파일 경로 출력
		::OutputDebugString(L"[Texture Error] Failed to load image path: ");
		::OutputDebugString(texturePath.c_str());
		::OutputDebugString(L"\n");
		return;
	}

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

void Texture::Render(ID2D1RenderTarget* renderTarget, Vector worldPos, Vector srcPos, Vector scale, bool flipX)
{
	if (!_bitmap) return;

	float destWidth = (float)_frameSizeX * scale.x;
	float destHeight = (float)_frameSizeY * scale.y;

	float left = worldPos.x;
	float top = worldPos.y;
	if (_applyCenter)
	{
		left -= (destWidth * 0.5f);
		top -= (destHeight * 0.5f);
	}

	D2D1_RECT_F destRect = D2D1::RectF(left, top, left + destWidth, top + destHeight);
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcPos.x,
		srcPos.y,
		srcPos.x + _frameSizeX,
		srcPos.y + _frameSizeY
	);

	if (flipX)
	{
		D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f),
			D2D1::Point2F(worldPos.x, worldPos.y)
		);
		renderTarget->SetTransform(scaleMatrix);
	}

	renderTarget->DrawBitmap(
		_bitmap,
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);

	if (flipX)
	{
		renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}

void Texture::Render(ID2D1RenderTarget* renderTarget, Vector worldPos, Vector srcPos, Vector size, Vector offset, Vector scale, bool flipX)
{
	if (!_bitmap) return;

	float destWidth = size.x * scale.x;
	float destHeight = size.y * scale.y;

	// 가운데 좌표기준으로 그림이 그려지게 보정해주자.
	Vector renderPos = worldPos + offset;
	
	float left = renderPos.x;
	float top = renderPos.y;
	if (_applyCenter)
	{
		left -= (destWidth * 0.5f);
		top -= (destHeight * 0.5f);
	}

	// 도착지
	D2D1_RECT_F destRect = D2D1::RectF(left, top, left + destWidth, top + destHeight);

	// 출발지(원본 스프라이트 시트에서 잘라낼 사각형 영역)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcPos.x,
		srcPos.y,
		srcPos.x + size.x,
		srcPos.y + size.y
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



void Texture::LoadWithPaletteSwap(wstring texturePath, const unordered_map<uint32, uint32>& colorMap, int32 row, int32 col, float dur, bool enableFlip)
{
	IWICImagingFactory* wicFactory = Game::GetInstance().GetWICFactory();
	ID2D1HwndRenderTarget* renderTarget = Game::GetInstance().GetRenderTarget();
	if (!wicFactory || !renderTarget) return;

	// 1. 디코더 생성 및 프레임(원본 이미지) 가져오기
	IWICBitmapDecoder* decoder = nullptr;
	wicFactory->CreateDecoderFromFilename(texturePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (!decoder) return;

	IWICBitmapFrameDecode* frame = nullptr;
	decoder->GetFrame(0, &frame);

	// 2. 32비트 포맷 변환기 생성 (Direct2D에서 사용하는 포맷인 BGRA 포맷으로 통일)
	IWICFormatConverter* converter = nullptr;
	wicFactory->CreateFormatConverter(&converter);
	converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);

	// 3. 수정 가능한 WICBitmap 생성 (원본 프레임은 읽기 전용이라 수정 불가능!)
	IWICBitmap* wicBitmap = nullptr;
	wicFactory->CreateBitmapFromSource(converter, WICBitmapCacheOnDemand, &wicBitmap);

	// 4. 픽셀 데이터 접근을 위해 Lock (메모리 잠금) 걸기
	IWICBitmapLock* lock = nullptr;
	WICRect rect = { 0, 0, (INT)0, (INT)0 };
	wicBitmap->GetSize((UINT*)&rect.Width, (UINT*)&rect.Height);
	wicBitmap->Lock(&rect, WICBitmapLockWrite, &lock);

	// 5. 픽셀 버퍼 배열과 버퍼의 총 크기(바이트) 가져오기
	UINT bufferSize = 0;
	BYTE* pixels = nullptr;
	lock->GetDataPointer(&bufferSize, &pixels);

	uint32* uPixels = (uint32*)pixels;

	// 픽셀을 순회하면서, 현재 픽셀 색상이 colorMap에 등록되어 있는지 검사합니다.
	for (int i = 0; i < (bufferSize / 4); ++i)
	{
		auto it = colorMap.find(uPixels[i]);
		// colorMap에서 현재 픽셀 색상을 찾았다면!
		if (it != colorMap.end())
		{
			// 해당 픽셀을 맵에 지정된 교체 색상으로 바꿉니다.
			uPixels[i] = it->second;
		}
	}
	
	// ----------------------

	// 6. Lock 해제 및 조작된 WIC 비트맵을 Direct2D 비트맵(_bitmap)으로 최종 변환
	lock->Release();
	renderTarget->CreateBitmapFromWicBitmap(wicBitmap, nullptr, &_bitmap);

	// 7. 텍스처 크기(Size) 멤버 변수 세팅 및 COM 객체 메모리 해제
	_bitmapSizeX = (uint32)_bitmap->GetSize().width;
	_bitmapSizeY = (uint32)_bitmap->GetSize().height;
	_col = col; _row = row; _dur = dur;
	_frameSizeX = _bitmapSizeX / _col; _frameSizeY = _bitmapSizeY / _row;
	_sizeX = _frameSizeX; _sizeY = _frameSizeY;

	wicBitmap->Release();
	converter->Release();
	frame->Release();
	decoder->Release();
}

// UI용
void Texture::RenderScreen(ID2D1RenderTarget* renderTarget, Vector screenPos, Vector srcPos)
{
	if (_applyCenter)
	{
		screenPos.x -= (_frameSizeX * 0.5f);
		screenPos.y -= (_frameSizeX * 0.5f);
	}

}
