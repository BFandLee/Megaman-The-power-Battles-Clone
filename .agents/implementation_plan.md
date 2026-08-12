# 보스 AI(Magnet Man) FSM 및 Phase 1 구현 계획 (최종)

`/grill-me` 인터뷰를 통해 보스 AI의 아키텍처 방향성이 모두 확정되었습니다.

## 📌 최종 확정된 아키텍처 설계
* **매크로 상태 관리 (FSM)**
  * `SpawnState` -> `Phase1State` -> (HP <= 50%) -> `Phase2State` -> (HP == 0) -> `DeadState`
  * 최상위 흐름만 통제합니다.
* **마이크로 패턴 제어 (Behavior Tree)**
  * `Phase1State`, `Phase2State` 내부에 탑재되어 세부 공격/이동 패턴을 결정합니다.
* **로직 캡슐화 (Option A 채택)**
  * 보스의 실제 행위(무적, 플레이어 끌어당기기, 점프 물리력 가하기 등)는 `Boss` 액터 클래스 내부의 멤버 함수(예: `SetInvincible()`, `PullPlayer()`)로 구현됩니다.
  * C++ BT Action 노드는 오직 이 함수들을 호출(위임)만 수행합니다.
* **블랙보드(Blackboard) 패턴 도입**
  * 여러 노드(공용 노드 포함) 간의 데이터 공유와 결합도 감축을 위해 `Blackboard` 클래스를 도입합니다.
  * C++의 캐스팅 에러를 방지하기 위해, `map<string, float>`, `map<string, Vector>`, `map<string, Object*>` 등 데이터 타입별로 맵(Map)을 나누어 관리합니다.
* **BT 노드의 3단계 모듈화 (Categorization)**
  * 범용성에 따라 노드들을 3가지로 분류하며, 클래스명 접두사 및 폴더로 명확히 구분합니다.
    1. **공용 (Common):** 모든 몬스터가 사용할 수 있는 노드 (예: `BTAction_Common_MoveToTarget`)
    2. **보스 전용 (Boss):** 특정 보스의 여러 페이즈에서 재사용되는 노드 (예: `BTAction_Boss_Shield`)
    3. **페이즈 전용 (Phase):** 특정 페이즈에서만 쓰이는 단발성 패턴 (예: `BTAction_Phase1_MagnetPull`)
* **BT 실행 방식**
  * Action 노드는 애니메이션이나 이동이 끝날 때까지 `Running`을 리턴하다가 끝나면 `Success`를 리턴합니다.
  * BT 트리 자체는 ImGui 에디터에서 조립하고 JSON으로 저장하여 사용합니다.

## Proposed Changes (실행할 스켈레톤 작업들)

### 1. Boss Actor (Magnet Man) 생성
* `Actor`를 상속받는 `Boss` 클래스 생성
* `FSMComponent`, `BTComponent` 부착
* 체력(HP) 변수 추가 및 `TakeDamage` 재정의
* **(추가됨) 패턴용 인터페이스 함수 선언:** `SetInvincible(bool)`, `PullPlayer()`, `ShootMagnetMissile()`, `JumpTo(Vector dest)` 등

### 2. FSM Component & Macro States
* `BossSpawnState`, `BossPhase1State`, `BossPhase2State`, `BossDeadState` 생성
* FSM 업데이트 시 HP에 따른 Phase 전환 로직 작성

### 3. Phase 1 커스텀 BT Node 클래스 생성 (새로운 분류 체계 적용)
* 하드코딩된 트리 구조 대신, ImGui에서 꺼내 쓸 수 있도록 개별 **노드 클래스(Node Class)**를 스켈레톤으로 생성합니다.
* **생성할 노드 클래스 (예시):**
  * **(공용)** `BTCheck_Common_Cooldown` (Condition 노드)
  * **(공용)** `BTAction_Common_JumpTo` (Action 노드 - 타겟이나 특정 좌표로 점프)
  * **(보스 전용)** `BTAction_Boss_MagnetShield` (Action 노드 - 보스의 공통 방어 로직)
  * **(페이즈 전용)** `BTAction_Phase1_MagnetPull` (Action 노드 - Phase1 특수 끌어당기기)
  * **(페이즈 전용)** `BTAction_Phase1_MagnetMissile` (Action 노드)

## Verification Plan
1. 사용자께서 위 작업(클래스 스켈레톤 및 구체 로직 작성)을 진행합니다.
2. 씬(Scene)에 `Boss`를 배치합니다.
3. ImGui BT 에디터를 열고 생성해둔 커스텀 노드들을 사용해 Phase 1 트리를 시각적으로 조립합니다.
4. 보스가 `Phase1State`에 진입했을 때, 에디터에서 만든 트리가 `Running` 상태를 잘 유지하며 패턴을 수행하는지 확인합니다.
