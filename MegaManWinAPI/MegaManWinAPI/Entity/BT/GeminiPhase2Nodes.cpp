#include "pch.h"
#include "GeminiPhase2Nodes.h"
#include "Blackboard.h"
// #include "SceneManager.h" 
// #include "GeminiLaserProjectile.h" // 레이저 투사체 클래스 (구현 예정)

NodeState BTAction_Gemini_CloneDeactivate::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 2: 분신 해제
    // Blackboard에 저장된 분신 객체 포인터를 가져와 Destroy 처리합니다.
    return NodeState::Success; 
}

NodeState BTAction_Gemini_CloneSummon::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 2: 분신 소환 (Activate와 유사)
    return NodeState::Success; 
}

NodeState BTAction_Gemini_LaserMissile::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 2: 레이저 미사일 발사
    // 1. SceneManager를 통해 `GeminiLaserProjectile` 액터를 Spawn합니다.
    // 2. 발사 방향을 45도 아래 등 튕기기 좋은 각도로 설정합니다.
    // (물리 및 반사 연산은 Projectile 자체 Update에서 수행하도록 위임합니다)
    return NodeState::Success; 
}

NodeState BTAction_Gemini_SequentialJump::Tick(Blackboard* bb)
{
    // [코칭 가이드] Phase 2: 연속 점프 이동
    // 점프를 두 번 연속으로 수행한 뒤 이동하는 시퀀스의 구체적 물리/상태 전이를 제어합니다.
    return NodeState::Success; 
}
