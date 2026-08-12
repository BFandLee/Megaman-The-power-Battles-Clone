#include "pch.h"
#include "ConditionDecorator.h"
#include "Blackboard.h"

NodeState ConditionDecorator::Tick(Blackboard* bb)
{
    // 조건이 참일 때만 자식 노드를 실행
    if (checkCondition(bb))
    {
        if (_child != nullptr)
        {
            return _child->Tick(bb);
        }
    }
    
    return NodeState::Failure;
}
