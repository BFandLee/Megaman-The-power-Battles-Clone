# Week 3: 데이터 주도형 보스 패턴 (Visual BT Editor 연동)

아하, 코치님의 의도를 정확히 파악했습니다! 단순히 코드로 하드코딩해서 트리를 엮는 것이 아니라, **BTEditor에서 시각적으로 노드를 생성/연결하고, 이를 JSON으로 저장한 뒤 인게임에서 불러와서 실행하는 "데이터 주도형(Data-Driven) 행동 트리"**를 원하셨군요. 상용 게임 엔진(언리얼, 유니티)과 동일한 아주 훌륭한 접근 방식입니다!

## 📌 목표 아키텍처 (Visual Editor Workflow)

1. **에디터 조립 (Visual Edit)**: `BTEditor` 화면에서 마우스 우클릭으로 노드(Selector, Action 등)를 생성하고 핀(Pin)을 드래그해 연결합니다.
2. **저장 (Serialization)**: [Save] 버튼을 누르면 완성된 트리의 연결 구조가 `BTSerializer`를 통해 `BT_BossPhase1.json` 등의 파일로 저장됩니다.
3. **인게임 로드 (Deserialization)**: 옐로우 데빌이 `Phase1State(BTState)`에 진입할 때, 코드로 노드를 생성하는 것이 아니라 `BTSerializer::LoadFromJSON("BT_BossPhase1.json")`을 호출하여 파일로부터 트리를 복원(Rebuild)해 냅니다.
4. **실행 (Execution)**: 매 프레임 `_rootNode->Tick(bb)`이 호출되며 복원된 트리가 동작합니다.

---

## 🛠️ 제안하는 구현 단계 (Proposed Changes)

이 계획에 따라 다음 순서대로 코칭을 진행하겠습니다. 프로젝트 제약(Raw Pointer 사용)과 1개월 차 C++ 학습 목표를 고려하여 단계를 세분화했습니다.

### 1. BTEditor: 시각적 노드 생성과 연결
- 하드코딩된 노드를 제거하고 ImGui 우클릭 메뉴(`ImGui::OpenPopup`)를 통해 동적 노드(Selector, Action 등) 생성 리스트를 구현합니다.
- 노드 간 선(Link)을 긋고 상태를 저장하는 로직(`ImNodes::IsLinkCreated`)을 추가합니다.

### 2. JSON 직렬화 및 팩토리 패턴 (Serializer & Factory)
- **저장(Serialization):** 에디터의 노드와 링크 정보를 JSON으로 저장합니다. (외부 라이브러리 활용 여부 확인 필요)
- **동적 로드(Factory Pattern):** 리플렉션이 없는 C++에서 JSON의 문자열 타입(`"Selector"`, `"BossAction_Shoot"`)을 읽어 실제 클래스를 `new`로 할당하는 팩토리 로직을 직접 설계해 봅니다.
- **메모리 해제(Memory Management):** 스마트 포인터 금지 규칙에 따라, 할당된 트리의 Root 노드 삭제 시 전체 트리가 메모리 누수 없이 안전하게 해제되도록 소멸자 구조를 설계합니다.

### 3. Blackboard(칠판) 설계
- 복잡한 범용 컨테이너(예: std::any) 대신, 보스의 상태를 직관적으로 담을 수 있는 전용 강타입 구조체(`BossBlackboard`)를 설계하여 트리에 전달합니다.

### 4. Boss 몬스터 적용 (FSM 연동)
- `Phase1State` 진입 시 `BTSerializer`를 통해 JSON 파일로부터 트리를 동적 로드합니다.
- 매 프레임 `Update()`에서 복원된 트리의 `Tick()`을 호출하여 동작을 확인합니다.

---

## 🙋‍♂️ User Review Required

> [!IMPORTANT]
> **첫 번째 미니 과제: JSON 라이브러리 준비**
> 
> 에디터에서 노드를 시각적으로 엮고 파일로 저장해 활용하는 파이프라인으로 방향을 확정했습니다.
> 이제부터 정답 코드를 바로 드리지 않고, 스스로 핵심 구조를 설계하실 수 있도록 질문과 힌트로 가이드하겠습니다.
> 
> **[과제 1]**
> JSON 직렬화를 위해서는 파서 라이브러리가 필요합니다. 현재 프로젝트에 세팅된 JSON 라이브러리(예: `RapidJSON`, `nlohmann/json` 등)가 있나요? 없다면 C++에서 널리 쓰이는 `nlohmann/json`을 헤더 파일 하나로 추가하는 것을 추천합니다. 준비 상황을 알려주세요!
