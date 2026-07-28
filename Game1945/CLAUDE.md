# CLAUDE.md

이 파일은 이 저장소에서 작업할 때 Claude Code(claude.ai/code)에게 제공되는 가이드입니다.

## 개요

Game1945는 Win32 API와 GDI만으로 만든 2D 종스크롤 슈팅 게임(1945류)으로, C++20으로 작성되었습니다. 별도의 게임 엔진이나 그래픽 라이브러리는 사용하지 않으며, 투명 블릿을 위한 `msimg32.lib` 정도만 사용합니다. 코드 전반의 주석은 한국어이며, 소스 파일은 UTF-8이 아닌 **CP949** 인코딩으로 저장되어 있습니다.

## 빌드

CMake/Makefile은 없으며 Visual Studio 솔루션입니다.

- `Game1945.sln`을 Visual Studio(v143 툴셋)로 열어 `Game1945` 프로젝트를 빌드하거나, 커맨드라인에서 빌드할 수 있습니다:
  ```
  msbuild Game1945.sln /p:Configuration=Debug /p:Platform=x64
  ```
- 빌드 결과물은 `Game1945\x64\Debug\`(또는 `Release`)에 생성됩니다.
- 빌드된 `Game1945.exe`를 직접 실행하면 되며, 실행 파일 기준 한 단계 위의 `Resources\` 디렉터리를 필요로 합니다(`ResourceManager`/`DataManager`는 경로를 `<exeDir>/../Resources/`로 계산합니다).
- 이 저장소에는 테스트 스위트가 없습니다.

## 중요: 인코딩 — 소스 파일에 Edit/Write 도구를 절대 사용하지 말 것

소스 파일(`.cpp`/`.h`)은 UTF-8이 아닌 **CP949**로 저장되어 있으며 한국어 주석을 포함합니다. Edit/Write 도구는 UTF-8로 디코딩·재인코딩하기 때문에, 파일을 열고 다시 쓰는 것만으로도 모든 한국어 주석이 대체 문자(replacement character)로 조용히 손상됩니다(방금 수정한 파일에서 낯선 `C4819` MSVC 경고가 뜨면 이 손상의 대표적인 징후입니다).

- **이 저장소의 어떤 `.cpp`/`.h` 파일에도 Edit나 Write 도구를 사용하지 마십시오.**
- 대신 PowerShell로 편집하십시오: `[System.Text.Encoding]::GetEncoding(949)`로 바이트를 읽고, ASCII 앵커에 대해 순서 기반(ordinal) `.Replace()`를 수행한 뒤, 같은 949 인코딩으로 `WriteAllBytes`를 사용해 다시 씁니다. 편집 후에는 `EF BF BD`(U+FFFD) 바이트 시퀀스가 새로 생기지 않았는지 반드시 확인하십시오.
- 새로 추가하는 주석은 CP949에서도 안전하도록 ASCII/영문으로 작성하십시오.

## 아키텍처

전형적인 컴포넌트/싱글톤 매니저 구조의 게임 루프를 따릅니다:

- **진입점**: `Game1945.cpp`(`wWinMain`)이 Win32 윈도우를 생성하고, 대기 중인 윈도우 메시지가 없을 때마다 `Game::Update()`/`Game::Render()`를 호출하는 수동 메시지 루프를 실행합니다. `QueryPerformanceCounter`를 이용해 120FPS를 목표로 프레임을 제한합니다.
- **`Game`**(`Game.h/.cpp`, `wWinMain`에서 구동되는 사실상의 싱글톤): 백버퍼 HDC/비트맵을 소유하여(`BitBlt`를 통한 더블 버퍼링) 모든 매니저와 `Scene`을 초기화·구동합니다. 매 프레임 고정된 순서로 처리됩니다: `TimeManager` → `InputManager` → `Scene::Update` → `CollisionManager::Update`, 그다음 `Scene::Render` → `CollisionManager::Render`.
- **싱글톤**(`Singleton.h`의 CRTP 베이스 `Singleton<T>`, 함수 지역 `static T instance` 사용): `TimeManager`(델타 타임/FPS), `InputManager`, `ResourceManager`(모든 `Texture*`를 문자열 키로 로드·소유), `DataManager`(JSON 기반 설정 데이터 로드), `CollisionManager`(그리드 기반 브로드 페이즈 + 원형 충돌 내로우 페이즈).
- **`Scene`**: 살아 있는 모든 `Actor*` 인스턴스를 소유하며, `RenderLayer`별 렌더 리스트(`_renderList[RenderLayer]`), 공간 분할을 위한 균일 그리드(`vector<GridInfo> _grid`, 셀 크기 `_gridSize`), 그리고 풀링되는 액터 타입을 위한 두 개의 `ObjectPool<T>` 인스턴스(`_bulletPool`, `_enemyPool`)를 갖습니다. 액터 추가/제거는 `_reservedAdd`/`_reservedRemove`를 통해 지연 처리되며, 컨테이너를 순회 도중 변경하지 않도록 안전한 시점에 반영됩니다.
- **`Actor`**(`Actor.h`): 월드에 배치되는 모든 것(`Airplane` → `Player`/`Enemy`, `Bullet`, `Background`, `Effect`)의 기반 클래스입니다. 위치, `_pendingKill` 플래그, `Component*` 목록(템플릿 `AddComponent<T>()`/`GetComponent<T>()`가 `dynamic_cast`를 사용)을 가지며, 풀링된 액터가 스스로 `Return()`할 수 있도록 `IObjectPool*` 역참조도 보유합니다. 서브클래스는 `GetRenderLayer()`와 `GetActorType()`을 반드시 구현해야 하며, 충돌 판정에 참여하려면 `GetCollider()`를 선택적으로 오버라이드합니다.
- **`Component`**(`Component.h`): `Actor`에 부착되는 최소 인터페이스(`Update`/`Render`)입니다. 구체 컴포넌트로는 `ImageRenderer`/`SpriteRenderer`(`Texture`를 통한 그리기), `ColliderCircle`(원-원 충돌, 액터별로 `SetCheckCell`을 통해 선택적으로 참여)이 있습니다.
- **`ObjectPool<T>`**(`ObjectPool.h`): 고정 크기의 `vector<T>`와 프리 리스트 `vector<T*>`를 미리 할당해 두고, `Acquire()`/`Return()`으로 인스턴스를 재사용하여 `new`/`delete`를 피합니다. 풀이 고갈되면 자동으로 확장하지 않고 assert가 발생하며, 풀 크기는 `Init(size)` 시점에 고정됩니다.
- **`CollisionManager`**: 매 프레임 충돌 중인 액터 쌍의 `_prev`/`_curr` set을 유지하여 `Actor`의 `OnEnter`/`OnStay`/`OnExit` 콜백을 도출하고, `IGNORE_MASK[ActorType][ActorType]` 매트릭스로 무관한 타입 쌍(예: 적 탄환 대 적)의 충돌 검사를 건너뜁니다.
- **데이터 기반 리소스**: `Resources\Data\` 아래의 `ResourceData.json`을 `DataManager`/`ResourceData`가 (vendored된 `Game1945\Json\nlohmann\json.hpp`의 nlohmann json을 통해) 로드하여 텍스처(`key`, `fileName`, 투명색, 스프라이트 시트 행/열 수, 애니메이션 지속 시간)를 기술합니다. 이후 `ResourceManager::LoadTexture`가 이 표를 기반으로 `Resources\`에서 실제 `.bmp`를 로드합니다.
- **좌표계 관련 참고**: `Scene::ConvertWorldToScreen`이 존재하는 이유는, Scene이 스크롤되는 `_cameraPos`/`_mapSize`와 함께 월드 공간 액터 위치를 추적하기 때문이며, 이는 HDC로 그리는 화면 공간 좌표와 별개입니다.
