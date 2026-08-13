---
name: Boss Behavior Tree ImGui Editor Architecture
description: 보스 AI 비헤이비어 트리와 ImGui 에디터 간의 데이터 연동 및 FSM 통합을 위한 설계 규칙입니다.
---

# Boss Behavior Tree & ImGui Editor 설계 규칙

본 지침은 ImGui 에디터를 활용하여 Boss AI 비헤이비어 트리(BT)를 구축하고 런타임에 실행할 때 준수해야 할 핵심 설계 규칙입니다.

## 1. 노드 속성(Property) 편집
- **규칙:** 각 BT 노드의 고유 속성(예: 쿨다운 시간, 확률 등)은 노드 클래스 내부에 선언된 `virtual void DrawProperty()` 함수를 오버라이딩하여 에디터 인스펙터(Inspector) UI에 노출합니다.
- **설계 의도:** 노드마다 필요한 속성이 다르므로, UI를 에디터(BTEditor) 측에서 하드코딩하지 않고 각 노드 객체가 스스로 자신의 속성 UI를 그리도록(Polymorphism) 위임합니다.

## 2. 직렬화(Serialization) 및 트리 조립(Loading)
- **저장(Save):** 에디터 상태를 저장할 때는 트리 구조가 아닌, '노드 배열(`_testNodes`)'과 '링크(연결선) 배열(`_links`)'의 **평면적(Flat) 구조**를 유지하여 직렬화합니다.
- **로드(Load) 및 조립:** 게임 런타임에서 해당 JSON 파일을 읽어올 때 `BTSerializer` 또는 전용 로더 빌더(Builder)가 링크 정보(`startAttrId`, `endAttrId`)를 파싱하여 부모 노드의 `AddChild()`를 호출함으로써, 메모리 상에 실제 트리 자료구조를 동적 구축(Build)합니다.

## 3. FSM과 BT의 결합
- **규칙:** `Boss` 액터 내부에서 직접 BT 전체를 들고 틱(Tick)하지 않습니다.
- **구조:** 상위 FSM의 각 페이즈 상태(예: `BossPhase1State`)가 해당 페이즈에 대응하는 전용 BT JSON 파일을 로드하고 `_rootNode`를 멤버로 소유합니다. 상태의 `Update()` 루프 내에서 해당 루트 노드를 실행(`Tick`)시킵니다.

## 4. 데이터 공유 및 Blackboard
- **규칙:** 보스의 트랜스폼(Transform), 플레이어 타겟, 체력 비율 등의 게임 데이터는 노드 생성자나 `Init`으로 개별 주입하지 않습니다.
- **구조:** 트리 실행 시 `RootNode->Tick(Blackboard* bb)` 인자로 블랙보드를 넘겨줍니다. 노드(주로 데코레이터나 액션)들은 `Blackboard`에 존재하는 맵(Map) 데이터나 헬퍼(Getter/Setter) 함수를 통해 중앙 집중식으로 데이터를 교환합니다.
