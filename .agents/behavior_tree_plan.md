# 설계안 2: Behavior Tree (행동 트리) 뼈대

이 설계안은 보스의 다채로운 패턴을 제어하기 위한 행동 트리(Behavior Tree) 컴포넌트들의 뼈대(Skeleton)를 정의합니다.

## ⚠️ User Review Required

> [!IMPORTANT]
> **스마트 포인터 사용 금지 및 지연 삭제(Delayed Deletion) 규칙**
> 모든 AI 노드 및 상태 객체는 `new`로 동적 할당되며, 생명 주기 관리는 전적으로 상위 객체(FSMComponent 또는 상위 BTNode)가 책임집니다.
> 프레임 실행 도중 메모리가 해제되는 것을 막기 위해 `bPendingKill` 플래그를 사용해 Update의 마지막 단계에서 안전하게 일괄 `delete`를 수행해야 합니다.

## 🛠️ Proposed Changes

### [NEW] `MegaManWinAPI/AI/Blackboard.h`
BT 노드 간 데이터 공유를 위한 클래스입니다.
```cpp
class Blackboard
{
public:
    // 공통적으로 필요한 데이터 포인터 저장
    class Actor* OwnerBoss = nullptr;
    class Actor* TargetPlayer = nullptr;

    // 추가적인 데이터(HP, 타이머 등)를 유연하게 저장하기 위해 map을 사용하거나 멤버 변수 추가 가능
};
```

### [NEW] `MegaManWinAPI/AI/BTNode.h`
모든 행동 트리 노드의 최상위 인터페이스입니다. 지연 삭제 기능과 ImGui 렌더링 기능을 포함합니다.
```cpp
enum class ENodeState
{
    Success,
    Failure,
    Running
};

class BTNode
{
public:
    BTNode() {}
    virtual ~BTNode() {}

    // 핵심 실행 함수
    virtual ENodeState Tick(class Blackboard* bb) = 0;

    // ImGui를 통한 시각화 함수 (디버깅용)
    virtual void RenderUI() = 0;

    // 노드 식별을 위한 이름 반환
    const std::string& GetName() const { return _name; }
    void SetName(const std::string& name) { _name = name; }

    // 노드의 마지막 실행 결과를 반환 (시각화 시 색상 표현 등에 사용)
    ENodeState GetLastState() const { return _lastState; }
    void SetLastState(ENodeState state) { _lastState = state; }

    // 지연 삭제(Delayed Deletion)를 위한 플래그
    void MarkPendingKill() { _bPendingKill = true; }
    bool IsPendingKill() const { return _bPendingKill; }

protected:
    std::string _name = "BTNode";
    ENodeState _lastState = ENodeState::Running;

private:
    bool _bPendingKill = false;
};
```

### [NEW] `MegaManWinAPI/AI/BTComposite.h`
Selector와 Sequence 노드의 기본형태입니다. (구현은 cpp에서 직접 작성하셔야 합니다)
```cpp
#include "BTNode.h"
#include <vector>

class BTComposite : public BTNode
{
public:
    virtual ~BTComposite(); // 자식 노드 순회하며 일괄 delete 필요

    void AddChild(BTNode* child) { _children.push_back(child); }

    // Composite 노드 공통 시각화 (ImGui::TreeNode 등을 활용해 자식 노드 순회 렌더링)
    virtual void RenderUI() override;

protected:
    // 지연 삭제 대상인 자식들을 정리하는 유틸 함수
    void CleanupPendingKillChildren(); 

    std::vector<BTNode*> _children;
};

// [Selector] 자식 중 하나라도 Success면 Success 반환
class BTSelector : public BTComposite
{
public:
    virtual ENodeState Tick(class Blackboard* bb) override;
};

// [Sequence] 자식 중 하나라도 Failure면 Failure 반환
class BTSequence : public BTComposite
{
public:
    virtual ENodeState Tick(class Blackboard* bb) override;
};
```
