#include "pch.h"
#include "GeminiPhase1Nodes.h"
#include "Blackboard.h"
// #include "SceneManager.h" // 씬 매니저 참조 (분신 Spawn용)
// #include "GeminiClone.h" // 분신 클래스 (구현 예정)

NodeState BTAction_Gemini_CloneActivate::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 1: 분신 활성화
    // 1. 여기서 제미니맨의 '분신(GeminiClone)' 액터를 새로 Spawn 해야 합니다. (예: SceneManager를 통해 추가)
    // 2. 분신 생성 후, 분신 객체의 포인터나 상태(분신이 활성화되었음)를 Blackboard에 저장해두면
    //    다른 노드(예: Phase 1 기본 이동 시 분신 체크 조건)에서 쉽게 상태를 파악할 수 있습니다.
    
    // (구현) ...

    return NodeState::Success; // 완료되면 SUCCESS 반환
}
