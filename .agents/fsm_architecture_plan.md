# 설계안 1: FSM 리팩토링 및 BT 상태 구조

이 설계안은 기존 플레이어 중심의 `FSMComponent`를 범용 상태 관리 컴포넌트로 리팩토링하고, 물리/이동 데이터를 분리하며, 보스용 행동 트리를 실행하기 위한 기본 상태(State) 인터페이스를 정의합니다.

## 💡 Design Decision

> [!TIP]
> **페이즈 전환 시점 처리: Observer 패턴 적용**
> 보스 액터가 데미지를 입었을 때 `FSMComponent`에게 상태 전환을 알리는 방식은 **Observer 패턴**을 사용하여 결합도를 낮추기로 결정했습니다. 보스의 체력이 변경될 때 이벤트를 발생시키고, 상태(State)가 리스너로 등록하여 이를 감지한 뒤 Phase 2로 전환하도록 구현할 예정입니다.

## 🛠️ Proposed Changes

### [MODIFY] `MegaManWinAPI/Entity/Component/FSMComponent.h`
기존에 플레이어용으로 존재하던 `FSMComponent`를 **순수 범용 상태 관리 컴포넌트**로 리팩토링합니다. 내부에 있던 물리/이동 변수들(`_movespeed`, `_jumpforce` 등)은 제거합니다.
`RenderUI()`는 오직 **현재 어떤 상태(State)가 실행 중인지, BT 노드의 실행 상태는 어떠한지(시각화)**를 그리는 데만 사용됩니다.
```cpp
// (기존 코드에서 이동 관련 변수 모두 제거)
class FSMComponent : public Component
{
    // ... 기존 상태 관리 코드 유지 ...
    virtual void RenderUI() override; // FSM 상태 전환 이력 및 BT 트리 시각화 전용
};
```

### [NEW] `MegaManWinAPI/Entity/Component/MovementComponent.h`
`FSMComponent`에서 분리된 물리 및 이동 변수들을 전담하는 새로운 컴포넌트입니다.
*(※ 네이밍 판단: 중력이나 질량 기반의 사실적인 물리 시뮬레이션이라면 `PhysicsComponent`가 맞지만, `_jumpforce`나 `_movespeed` 같은 록맨 특유의 아케이드성 조작감을 제어하는 목적이 강하므로 `MovementComponent`라는 이름이 구조상 더 적합합니다.)*

이 컴포넌트 역시 자체적으로 `RenderUI()`를 가져서, **이동 속도나 점프력 등의 물리 데이터를 실시간으로 보고 수정(Tweak)할 수 있도록** 만듭니다.
```cpp
class MovementComponent : public Component
{
public:
    // ... 생성자, Update 등 ...
    virtual void RenderUI() override; // 이동/물리 변수 실시간 모니터링 및 수정용 ImGui 렌더링

    float _movespeed = 0.0f;
    float _jumpforce = 0.0f;
    float _accel = 10.0f;
    float _currentaccel = 10000.0f;
    float _slideVelocity;
    float _friction;
};
```

### [NEW] `MegaManWinAPI/AI/BTState.h`
보스 전용 행동 트리(BT)를 실행하기 위해, 기존의 공용 `State` 인터페이스를 상속받은 클래스입니다.
```cpp
#include "Entity/FSM/State.h"

class BTState : public State
{
public:
    BTState(FSMComponent* pOwner) : State(pOwner) {}
    virtual ~BTState(); // 루트 노드(BTNode) 해제

    virtual void Enter(class Blackboard* bb) = 0;
    virtual void Update(float deltaTime) override; // 내부에서 _rootNode->Tick() 호출
    virtual void Exit() override;
    
    // ImGui 시각화를 위한 함수
    virtual void RenderUI(); 

    void SetRootNode(class BTNode* node) { _rootNode = node; }

protected:
    class BTNode* _rootNode;
};
```
