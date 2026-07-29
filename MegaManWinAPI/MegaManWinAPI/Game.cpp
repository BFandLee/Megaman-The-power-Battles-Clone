#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Util.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "DataManager.h"
#include "UIManager.h"
#include "SoundManager.h"

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;

	// 1. Direct2D 공장(Factory) 생성
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2dFactory);
	if (FAILED(hr)) return;

	// 2. 창의 클라이언트 영역 크기 구하기
	RECT rc;
	GetClientRect(hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// 3. HWND(윈도우 창)에 그림을 그릴 수 있는 렌더 타겟 생성
	hr = _d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&_renderTarget
	);

	// TimeManager
	TimeManager::GetInstance().Init();

	// InputManager
	InputManager::GetInstance().Init(hwnd);

	// SoundManager
	SoundManager::GetInstance().Init(hwnd);

	// full path c:// /// // /
	// 리소스 매니저 초기화
	wchar_t buffer[MAX_PATH];
	DWORD length = ::GetCurrentDirectory(MAX_PATH, buffer);
	fs::path currentPath = fs::path(buffer) / L"../Resources/";
	ResourceManager::GetInstance().Init(hwnd, currentPath);

	// DataManager 초기화
	// DataManager::GetInstance().Init(currentPath);
	// DataManager::GetInstance().Load();

	// Scene 초기화
	// SceneManager::GetInstance().Init();

	// CollisionManager 초기화
	CollisionManager::GetInstance().Init();
	// UIManager::GetInstance().Init();
}

void Game::Cleanup()
{
	// SceneManager::GetInstance().Cleanup();

	// 매니저들 각자 정리가 필요한것들은 정리해준다.
	ResourceManager::GetInstance().Cleanup();
}

void Game::Update()
{
	// 각종 업데이트 로직 처리
	TimeManager::GetInstance().Update();

	// 입력 업데이트 
	InputManager::GetInstance().Update();

	// Scene 업데이트
	// SceneManager::GetInstance().Update(TimeManager::GetInstance().GetDT());

	// 모든 Update가 끝나고 좌표 갱신이 완료된 후, 충돌체크 수행
	// UIManager::GetInstance().Update(TimeManager::GetInstance().GetDT());
	CollisionManager::GetInstance().Update();
}

void Game::Render()
{
	if (_renderTarget == nullptr) return;

	// [Direct2D 그리기 시작]
	_renderTarget->BeginDraw();

	// 매 프레임마다 이전 잔상을 지우기 위해 검은색(혹은 원하는 색)으로 화면 덮기
	_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));


	// 각종 렌더링 로직 처리
	// 
	// Scene의 모든 객체 렌더링
	// SceneManager::GetInstance().Render(_hdcBack);

	CollisionManager::GetInstance().Render(_renderTarget);

	// UIManager::GetInstance().Render(_hdcBack);
	
	// [Direct2D 그리기 종료]
	_renderTarget->EndDraw();
}

Game:: ~Game()
{
	// 스마트 포인터를 사용할 수 없으므로 생성한 순서의 역순으로 직접 Release() 호출
   if (_renderTarget) {
	   _renderTarget->Release();
	   _renderTarget = nullptr;
   }
   if (_d2dFactory) {
	   _d2dFactory->Release();
	   _d2dFactory = nullptr;
   }
}


