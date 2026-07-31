# ImGui Inspector (RenderUI) 도입 계획

지금까지 연동한 Dear ImGui를 활용하여, 게임 내 액터(캐릭터, 배경 등)의 세부 속성을 실시간으로 확인하고 수정할 수 있는 **런타임 인스펙터(Inspector) UI 구조**를 설계 및 적용합니다. 

## 설계 핵심 내용

* **하이어라키(Hierarchy) 방식 적용:** 화면에 현재 Scene에 존재하는 모든 Actor의 목록(트리 구조)을 띄우고, 확장하여 내부 컴포넌트 값을 조작할 수 있게 합니다.
* **이름 식별자(`_name`) 부여 (사용자 피드백 반영):** 인스펙터에서 각 객체를 식별하기 위해 `Actor`와 `Component` 클래스에 문자열 형태의 `_name` 변수를 추가합니다.
* **객체 지향적 분산 처리:** 최상위(Scene)에서는 UI 뼈대만 잡고, 실제 자신의 데이터를 그리는 역할은 `Actor`와 `Component`가 수행하도록 `virtual void RenderUI()` 패턴을 도입합니다.

## Proposed Changes

---

### 1. 식별자(_name) 추가
액터와 컴포넌트가 인스펙터 트리 구조에서 알아보기 쉬운 이름을 가지도록 수정합니다.

#### [MODIFY] [Actor.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Actor/Actor.h) & [Component.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Component/Component.h)
- `std::string _name;` (또는 `std::wstring _name;`) 멤버 변수 추가. (ImGui 호환성을 위해 가급적 `std::string` 사용 추천)
- Getter/Setter 또는 생성자 매개변수를 통해 이름을 지정할 수 있도록 구현.
- 예: `Player` 생성자에서 `_name = "Player";` 할당.

---

### 2. Component 계층
컴포넌트들이 스스로의 속성을 UI로 그릴 수 있도록 가상 함수를 추가합니다.

#### [MODIFY] [Component.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Component/Component.h)
- `virtual void RenderUI() {}` 가상 함수 추가 (기본 구현은 비워둠)

#### [MODIFY] [TransformComponent.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Component/TransformComponent.h) & [TransformComponent.cpp](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Component/TransformComponent.cpp)
- `RenderUI()` 오버라이드.
- `ImGui::DragFloat2` 등을 사용해 `_position`, `_scale` 등 트랜스폼 데이터를 슬라이더로 조절할 수 있도록 구현.

---

### 3. Actor 계층
액터는 자신이 소유한 컴포넌트 목록을 순회하며, 각 컴포넌트의 `RenderUI()`를 호출해줍니다.

#### [MODIFY] [Actor.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Actor/Actor.h) & [Actor.cpp](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Actor/Actor.cpp)
- `virtual void RenderUI()` 추가.
- 내부 구현에서 `ImGui::TreeNode(_name.c_str())`를 생성하여 트리를 열고, `_components` 벡터를 돌며 각 컴포넌트의 `RenderUI()`를 호출.
- 호출 후 `ImGui::TreePop()`으로 닫기.

---

### 4. Scene 및 Manager 계층
전체 씬 단위에서 인스펙터 창(Window)을 열고, 활성화된 액터 목록을 UI로 넘깁니다.

#### [MODIFY] [Scene.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Core/Scene.h) & [Scene.cpp](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Core/Scene.cpp)
- `void RenderUI()` 추가.
- `ImGui::Begin("Hierarchy")` 로 창을 열고, `_actors` 리스트를 돌며 각 액터별로 `actor->RenderUI()` 호출 후 `ImGui::End()` 호출.

#### [MODIFY] [SceneManager.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Manager/SceneManager.h) & [SceneManager.cpp](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Manager/SceneManager.cpp)
- `void RenderUI()` 추가. 내부적으로 현재 `_scene->RenderUI()`를 호출하도록 전달.

---

### 5. Game 계층
메인 게임 루프에서 ImGui 프레임 진행 중 `SceneManager`에게 UI 렌더링 지시를 내립니다.

#### [MODIFY] [Game.cpp](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Core/Game.cpp)
- `Game::Update()` 내에 있던 테스트용 `ImGui::ShowDemoWindow();` 코드를 `SceneManager::GetInstance().RenderUI();` 로 대체.

## Verification Plan

### Manual Verification
1. 프로젝트를 빌드하고 `F5`로 실행합니다.
2. 좌측 또는 우측에 "Hierarchy"라는 이름의 윈도우가 정상적으로 나타나는지 확인합니다.
3. 인스펙터 내에 "Player", "Background" 등 명시된 `_name`을 가진 액터들이 트리 형태로 나오는지 확인합니다.
4. 액터 트리를 확장(Expand)하여 `TransformComponent` 내부의 Position(위치) 슬라이더를 드래그해 봅니다.
5. 위치 슬라이더 조작 시, 화면 속 메가맨이나 배경의 실제 위치가 실시간으로 부드럽게 이동하는지 검증합니다.
