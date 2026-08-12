#include "pch.h"
#include "CooldownDecorator.h"
#include "Blackboard.h"
#include "TimeManager.h" // TODO: TimeManager::GetInstance()->GetDeltaTime() 사용을 위해 필요할 수 있습니다.

bool CooldownDecorator::checkCondition(Blackboard* bb)
{
    // TODO: 사용자가 직접 구현해 보세요!
    // 1. _timer에 DeltaTime을 더하거나 뺍니다.
    // 2. _timer가 _cooldownTime에 도달했는지 확인합니다.
    // 3. 도달했다면 _timer를 초기화하고 true를 반환합니다.
    // 4. 아직 도달하지 않았다면 false를 반환합니다.
    
    return true; // 임시 반환값
}
