# 플레이어 체력바 (PlayerHpBarUI) 구현 계획

플레이어의 타원형 체력바를 `UIElement` 기반의 클래스로 설계하고 구현합니다.
8개 세그먼트(칸)와 3단 레이어(투명 베이스, 빨간색, 노란색) 구조를 바탕으로 록맨 파워배틀 원작 방식의 체력 연출 및 25% 이하 위기 깜빡임 연출을 구성합니다.

---

## 📐 핵심 설계 사항

### 1. 3단 레이어 및 8개 세그먼트 메커니즘
- **레이어 계층**:
  1. **맨 밑**: 투명/빈 베이스 프레임 (`Hpbar_1`)
  2. **중간**: 빨간색 게이지 (`Hpbar_2`)
  3. **제일 위**: 노란색 게이지 (`Hpbar_3`)
- **8개 세그먼트(칸) & 총 16단계 HP 해상도**:
  - 타원 링을 구성하는 8개 블록 (총 16개 세부 스텝)
- **등장 연출 (Fill-up Animation)**:
  - 씬 시작 시 투명 베이스 틀 위에 **반시계 방향**으로 한 칸씩 `[투명] -> [빨간색] -> [노란색]` 순서로 게이지가 차오름.
- **체력 감소 연출 (Depletion)**:
  - 피격 시 **시계 방향**으로 한 칸씩 `[노란색] -> [빨간색] -> [투명]` 순서로 게이지가 소모됨.
- **위기 상태 (HitBar) 연출**:
  - 체력이 **25% 이하**일 때 붉은색 외곽 링 테두리(`HitBar_1`)가 주기적으로 깜빡임(Blink).

---

## 🛠️ 제안된 변경 사항

### [UI 컴포넌트 추가]

#### [NEW] [PlayerHpBarUI.h](file:///D:/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/UI/PlayerHpBarUI.h)
- `UIElement`를 상속받는 `PlayerHpBarUI` 클래스 선언
- 8개 세그먼트의 Source Bounding Box 좌표 정의 (반시계/시계 인덱스 매핑)
- 세그먼트 상태 (`Empty`, `Half_Red`, `Full_Yellow`)
- 텍스처 포인터 (`_baseTexture`, `_redTexture`, `_yellowTexture`, `_hitBarTexture`)
- 플레이어 타겟 포인터 (`_player`), 애니메이션/깜빡임 관련 상태 변수

#### [NEW] [PlayerHpBarUI.cpp](file:///D:/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/UI/PlayerHpBarUI.cpp)
- `Init()`: `ResourceManager`를 통해 관련 텍스처 획득
- `Update(float deltaTime)`:
  - 플레이어의 체력 계산 및 등장 Fill-up 보간 (`_displayHpRatio`)
  - 25% 이하 시 HitBar 깜빡임 타이머 갱신
- `Render(ID2D1RenderTarget* renderTarget)`:
  - 1단계: 베이스 틀(`Hpbar_1`) 렌더링
  - 2단계: `_displayHpRatio`에 따라 반시계/시계 방향 순서로 8개 칸의 빨간색/노란색 세그먼트 부분 렌더링
  - 3단계: 체력 25% 이하 시 깜빡임 주기에 맞춰 `HitBar_1` 렌더링

#### [MODIFY] [TestScene.cpp](file:///D:/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Core/TestScene.cpp)
- `PlayerHpBarUI` 인스턴스 생성 및 플레이어 타겟 연결 (`SetTarget(player)`)
- `UIManager::GetInstance().AddUI(playerHpBar)` 등록

---

## 🔍 검증 계획

1. **빌드 검증**:
   - 솔루션 빌드 오류 없이 컴파일되는지 확인
2. **동작 검증**:
   - 게임 시작 시 투명 베이스 틀 위에 반시계 방향으로 `[투명 -> 빨강 -> 노랑]` 순으로 차오르는지 확인
   - 피격 시 시계 방향으로 `[노랑 -> 빨강 -> 투명]` 순으로 소모되는지 확인
   - 체력 25% 이하 시 붉은 외곽 HitBar 링이 깜빡거리는지 확인
