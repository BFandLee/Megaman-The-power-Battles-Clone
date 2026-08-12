#include "pch.h"
#include "ProbabilityDecorator.h"
#include "Blackboard.h"
#include <stdlib.h> // rand() 함수 사용을 위해 필요할 수 있습니다.

bool ProbabilityDecorator::checkCondition(Blackboard* bb)
{
    // TODO: 사용자가 직접 구현해 보세요!
    // 1. rand() 함수 등을 이용해 0~99 사이의 랜덤 값을 뽑습니다.
    // 2. 뽑힌 값이 _chance 보다 작으면 true(성공), 크거나 같으면 false(실패)를 반환합니다.
    // 예: _chance가 30일 때, 랜덤 값이 0~29면 true, 30~99면 false
    
    return true; // 임시 반환값
}
