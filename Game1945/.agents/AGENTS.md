# Windows API Game1945 프로젝트 컨텍스트 & 개발 가이드라인

본 문서는 C++20 및 Windows API(GDI) 기반의 2D 탑다운 슈팅 게임인 **Game1945** 프로젝트의 아키텍처, 코딩 표준, 핵심 시스템을 정리한 문서입니다. 코드를 수정하거나 기능을 확장할 때 본 가이드라인을 참조하고 준수해 주십시오.

---

## 1. 프로젝트 개요 및 기술 스택
* **언어 및 표준**: C++20 (`std::format`, `std::filesystem` 등의 최신 기능 적극 활용).
* **프레임워크**: Win32 API 및 GDI(Graphics Device Interface) 렌더링.
* **렌더링 방식**: 화면 깜빡임 방지를 위해 더블 버퍼링(`HDC`, `HBITMAP` 메모리 디바이스 컨텍스트) 적용. 투명 배경 드로잉은 `TransparentBlt`를 사용하며, `msimg32.lib`가 링크되어 있습니다.
* **데이터 직렬화**: `nlohmann/json` 라이브러리를 사용한 JSON 기반 설정 데이터 연동.

---

## 2. 핵심 아키텍처 및 디자인 패턴

### 2.1 싱글톤 패턴 (Singleton Pattern)
모든 매니저(Manager) 클래스는 템플릿 클래스 `Singleton<T>`를 상속받습니다.
* **준수 사항**:
  - 자식 매니저 클래스의 생성자와 소멸자는 `private` 또는 `protected`로 선언해야 합니다.
  - 템플릿 기저 클래스가 자식 클래스를 인스턴스화할 수 있도록 `friend Singleton<매니저클래스명>;`을 반드시 추가해야 합니다.
* **예시**:
  ```cpp
  class TimeManager : public Singleton<TimeManager>
  {
      friend Singleton<TimeManager>;
  private:
      TimeManager() = default;
      ~TimeManager() = default;
  // ...
  };
  ```

### 2.2 컴포넌트 기반 액터 모델 (Component-Based Actor Model)
모든 게임 엔티티는 [Actor](file:///d:/Source/Main/Game1945/Game1945/Actor.h) 클래스를 상속받습니다. 액터는 자신의 수명을 관리하며, 컴포넌트를 통해 기능을 모듈화하여 조립할 수 있습니다.
* **컴포넌트 등록 및 조회**: 액터에 컴포넌트를 추가할 때는 `AddComponent<T>()`를, 가져올 때는 `GetComponent<T>()`를 사용합니다.
* **렌더링**: 비주얼 출력은 `ImageRenderer`나 `SpriteRenderer`와 같은 컴포넌트가 담당합니다.
* **수명 상태**: 프레임 루프 도중 안전한 객체 정리를 위해, 즉시 삭제하는 대신 `_pendingKill` 플래그를 세워 프레임 종료 시점에 지연 삭제 처리합니다.

### 2.3 씬 관리 (Scene Management)
[Scene](file:///d:/Source/Main/Game1945/Game1945/Scene.h) 클래스는 활성화된 액터들, 렌더링 레이어, 카메라 위치, 그리고 오브젝트 풀을 관리합니다.
* **안전한 추가/제거**: 루프 반복 도중 벡터가 수정되는 문제를 방지하기 위해, 대기 큐(`_reservedAdd` / `_reservedRemove`)를 거쳐 안전하게 액터를 추가 및 제거합니다.
* **렌더 레이어**: 액터들은 `RenderLayer`(Background, Enemy, Bullet, Player, Effect)로 분류되며, 뒤에서부터 앞으로 정렬되어 그려집니다.
* **공간 분할 격자(Grid)**: 충돌 연산 최적화를 위해 2D 그리드 구조(`Cell` & `GridInfo`)를 사용하여 충돌 쿼리를 효율적으로 수행합니다.

### 2.4 충돌 시스템 (Collision System)
[CollisionManager](file:///d:/Source/Main/Game1945/Game1945/CollisionManager.h)는 원형 영역(`ColliderCircle`) 간의 충돌을 판정합니다.
* **상태 관리**: 프레임 간의 오버랩 상태를 추적하여 아래의 상태 콜백을 호출합니다:
  - `OnEnter(Actor* other)`: 충돌이 시작되는 첫 프레임에 호출됩니다.
  - `OnStay(Actor* other)`: 충돌 상태가 유지되는 프레임 동안 지속적으로 호출됩니다.
  - `OnExit(Actor* other)`: 충돌이 끝나는 시점의 프레임에 호출됩니다.
* **충돌 마스크 (Collision Mask)**: 특정 타입 간의 불필요한 충돌체크(예: Player vs. PlayerBullet)를 필터링하기 위해 `IGNORE_MASK` 배열로 구성되어 있습니다.

### 2.5 오브젝트 풀링 (Object Pooling)
총알(`Bullet`), 적(`Enemy`), 이펙트(`Effect`)와 같이 생성과 소멸이 빈번하고 성능에 직간접적인 영향을 주는 엔티티는 [ObjectPool](file:///d:/Source/Main/Game1945/Game1945/ObjectPool.h)을 통해 관리하여 런타임 동적 할당 오버헤드를 방지합니다.
* **인터페이스**: `IObjectPool`을 상속받아 `Return(Actor*)`을 통해 동일한 방식으로 메모리를 환원합니다.
* **준수 사항**: 씬을 설정할 때 오브젝트 풀의 용량을 넉넉하게 초기화해야 합니다. 만약 풀이 고갈되면 단언문(assert)이 발생하도록 설계되어 있습니다.

---

## 3. 주요 클래스 및 데이터 구조

| 클래스명 | 소스 파일 | 설명 |
| :--- | :--- | :--- |
| **Game** | [Game.h](file:///d:/Source/Main/Game1945/Game1945/Game.h) | 애플리케이션의 메인 컨트롤러로, 윈도우 핸들(`HWND`) 관리, 메인 게임 루프 실행, GDI 더블 버퍼링을 담당합니다. |
| **Scene** | [Scene.h](file:///d:/Source/Main/Game1945/Game1945/Scene.h) | 전체 액터 리스트, 공간 분할 그리드, 오브젝트 풀, 카메라 좌표를 관리합니다. |
| **Actor** | [Actor.h](file:///d:/Source/Main/Game1945/Game1945/Actor.h) | 모든 게임 엔티티의 최상위 클래스로, 컴포넌트 부착과 수명 주기를 제어합니다. |
| **CollisionManager** | [CollisionManager.h](file:///d:/Source/Main/Game1945/Game1945/CollisionManager.h) | 원형 콜라이더 간의 충돌 감지 및 충돌 이벤트(Entry/Stay/Exit) 라이프사이클을 관리합니다. |
| **TimeManager** | [TimeManager.h](file:///d:/Source/Main/Game1945/Game1945/TimeManager.h) | 프레임 시간 간격(`GetDT()`), 프레임 레이트(FPS) 측정, 그리고 예약 지연 타이머 기능을 제어합니다. |
| **ResourceManager** | [ResourceManager.h](file:///d:/Source/Main/Game1945/Game1945/ResourceManager.h) | 외부 텍스처, 투명 비트맵 데이터 로드 및 리소스 수명을 관리합니다. |

### 핵심 수학 및 헬퍼 구조체 (`pch.h`에 정의됨)
* **Vector**: 위치와 방향을 나타내는 2D float 벡터 구조체입니다. 벡터의 덧셈, 뺄셈, 실수 곱하기 연산을 제공하며 `Length()`, `Normalize()`, `Dot()` (내적), `Cross()` (외적), `Rotate()` (회전) 등의 내장 수학 함수를 포함합니다.
* **Cell**: 공간 분할 격자 좌표의 인덱스 `iX`, `iY`를 나타내는 구조체입니다.
* **화면 해상도**: `pch.h`에 전역 상수 `GWinSizeX = 480`, `GWinSizeY = 800`으로 화면 크기가 고정 정의되어 있습니다.

---

## 4. 개발 가이드라인 및 제약 사항

1. **메모리 할당 및 해제**:
   * 활성화된 `Actor`에 대해 **절대로** 수동으로 `delete`를 호출해서는 안 됩니다. 대신 `Scene::DeleteActor(actor)` 또는 `actor->Destroy()`를 사용하여 지연 삭제 플래그(`_pendingKill`)로 관리해야 합니다.
   * 총알, 이펙트 등 자주 생성되고 소멸되는 임시 객체는 반드시 **오브젝트 풀(ObjectPool)**을 거쳐 생성 및 환원해야 하며, 런타임 중에 날(Raw) `new`/`delete` 호출을 지양합니다.

2. **GDI 그래픽 처리**:
   * 이미지 투명 렌더링 시 반드시 `TransparentBlt`를 사용하며, 투명색 키 값은 기본적으로 마젠타(RGB Magenta: `255, 0, 255`)를 사용합니다.

3. **충돌 판정 등록**:
   * 충돌 검사가 필요한 모든 액터는 `CollisionManager::AddActor(actor)`를 통해 검사 리스트에 등록되어야 하며, 해당 액터의 `GetCollider()` 함수가 유효한 `ColliderCircle` 주소를 반환해야 합니다.

4. **파일 인코딩 및 주석**:
   * 소스 파일 내 기존 한국어 주석을 유지하고 존중하십시오. 편집 후 파일 저장 시 시스템 기본 인코딩 또는 UTF-8 규격을 준수하여 한글 주석이 깨지지 않도록 하십시오.
