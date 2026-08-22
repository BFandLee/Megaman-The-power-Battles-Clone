#pragma once
#include "Singleton.h"
#include "pch.h"

// 전체 게임 로직을 담당하는 클래스
class Game : public Singleton<Game>
{
	// Singleton 객체를 '친구'로 선언해서 private 접근 가능하게 열어준다.
	friend Singleton<Game>;

public:
	void Init(HWND hwnd);
	void Cleanup();

	void Update();
	void Render();

	ID2D1HwndRenderTarget* GetRenderTarget() const { return _renderTarget; }
	IWICImagingFactory* GetWICFactory() const { return _wicFactory; }
	struct IDWriteFactory* GetDWriteFactory() const { return _dwriteFactory; }

	void ResizeWindow(int32 width, int32 height);
private:
	// 아무나 생성못하게 생성자/소멸자를 숨기자
	Game() = default;
	~Game() = default;

private:
	HWND _hwnd;	// 윈도우 핸들
	RECT _rect;		// 윈도우 크기

	ID2D1Factory* _d2dFactory = nullptr;
	ID2D1HwndRenderTarget* _renderTarget = nullptr;

	// WIC 이미징 팩토리 맴버 변수 추가
	IWICImagingFactory* _wicFactory = nullptr;
	
	// DirectWrite Factory (for ImGui)
	struct IDWriteFactory* _dwriteFactory = nullptr;
};
