#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Util.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "SceneManager.h"
#include "CollisionManager.h"
// #include "DataManager.h"
// #include "UIManager.h"
#include "SoundManager.h"
#include "ObjectPoolManager.h"
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

#ifdef _DEBUG
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_d2d.h"
#include "BTEditor.h"
#endif


void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	::GetClientRect(hwnd, &_rect);

	// COM 초기화
	::CoInitialize(nullptr);

	// 1. Direct2D 공장(Factory) 생성
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2dFactory);
	if (FAILED(hr)) return;

	// 2. WIC 공장 생성
	hr = ::CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_wicFactory)
	);
	if (FAILED(hr)) return;

	// 3. 렌더 타겟 생성
	RECT rc;
	GetClientRect(hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// 3. HWND(윈도우 창)에 그림을 그릴 수 있는 렌더 타겟 생성
	hr = _d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&_renderTarget
	);

	// DirectWrite Factory creation
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&_dwriteFactory));

#ifdef _DEBUG
	// ImGui Initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplD2D_Init(_renderTarget, _dwriteFactory, _wicFactory);
#endif

	// TimeManager
	TimeManager::GetInstance().Init();

	// InputManager
	InputManager::GetInstance().Init(hwnd);

	// SoundManager
	SoundManager::GetInstance().Init(hwnd);

	// 실행 파일 위치 기준으로 작업 디렉토리 고정
	wchar_t exePath[MAX_PATH];
	::GetModuleFileNameW(nullptr, exePath, MAX_PATH);
	fs::path exeDir = fs::path(exePath).parent_path();
	::SetCurrentDirectoryW(exeDir.c_str());

	// 리소스 매니저 초기화
	fs::path currentPath = exeDir / L"Resources/";
	ResourceManager::GetInstance().Init(hwnd, currentPath);
	ResourceManager::GetInstance().LoadAllSoundsInDirectory(exeDir / L"Resources/Sounds");

	// Scene 초기화
	SceneManager::GetInstance().Init();

	// CollisionManager 초기화
	CollisionManager::GetInstance().Init();

	ObjectPoolManager::GetInstance().Init();

	// UIManager::GetInstance().Init();

#ifdef _DEBUG
	BTEditor::GetInstance().Init();
#endif
}

void Game::Cleanup()
{
	// 생성의 역순으로 해제
	// 공장에서 렌더타겟이 생성됬기 때문에 렌더타겟을 먼저 해제하고 부모인 공장을 해제하는 설계(LIFO)
	if (_dwriteFactory)
	{
		_dwriteFactory->Release();
		_dwriteFactory = nullptr;
	}
	if (_wicFactory)
	{
		_wicFactory->Release();
		_wicFactory = nullptr;
	}
	if (_renderTarget)
	{
		_renderTarget->Release();
		_renderTarget = nullptr;
	}
	if (_d2dFactory)
	{
		_d2dFactory->Release();
		_d2dFactory = nullptr;
	}
	// COM 라이브러리 종료
	::CoUninitialize();

	SceneManager::GetInstance().Cleanup();

	// 매니저들 각자 정리가 필요한것들은 정리해준다.
	ResourceManager::GetInstance().Cleanup();

	ObjectPoolManager::GetInstance().Clear();

#ifdef _DEBUG
	ImNodes::DestroyContext();
	ImGui::DestroyContext();
#endif
}

void Game::Update()
{
	// 각종 업데이트 로직 처리
	TimeManager::GetInstance().Update();

	// 입력 업데이트 
	InputManager::GetInstance().Update();

	// Scene 업데이트
	SceneManager::GetInstance().Update(TimeManager::GetInstance().GetDT());

	// 모든 Update가 끝나고 좌표 갱신이 완료된 후, 충돌체크 수행
	// UIManager::GetInstance().Update(TimeManager::GetInstance().GetDT());
	CollisionManager::GetInstance().Update();

#ifdef _DEBUG
	ImGui_ImplD2D_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("FPS");
	ImGui::Text("fps : %d", TimeManager::GetInstance().GetFPS());
	ImGui::End();

	SceneManager::GetInstance().RenderUI();

	BTEditor::GetInstance().Update();
#endif
}

void Game::Render()
{
	if (_renderTarget == nullptr) return;

	// [Direct2D 그리기 시작]
	_renderTarget->BeginDraw();

	// 매 프레임마다 이전 잔상을 지우기 위해 검은색(혹은 원하는 색)으로 화면 덮기
	_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));


	// 각종 렌더링 로직 처리
	// 
	// Scene의 모든 객체 렌더링
	SceneManager::GetInstance().Render(_renderTarget);

	CollisionManager::GetInstance().Render(_renderTarget);

#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplD2D_RenderDrawData(ImGui::GetDrawData());
#endif

	// UIManager::GetInstance().Render(_hdcBack);
	
	// [Direct2D 그리기 종료]
	_renderTarget->EndDraw();
}

void Game::ResizeWindow(int32 width, int32 height)
{
	if (_hwnd == nullptr || _renderTarget == nullptr)
		return;

	// 1. 원하는 클라이언트 해상도에 맞춰 윈도우 전체(외곽선 포함) 크기 계산
	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	// 2. 윈도우 창 크기 변경 (위치는 그대로 두고 크기만 변경)
	::SetWindowPos(_hwnd, nullptr, 0, 0, windowWidth, windowHeight,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	// 3. Direct2D 렌더 타겟 크기 리사이징
	_renderTarget->Resize(D2D1::SizeU(width, height));
}


