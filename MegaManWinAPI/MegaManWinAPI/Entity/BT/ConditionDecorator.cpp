#include "pch.h"
#include "ConditionDecorator.h"
#include "Blackboard.h"

NodeState ConditionDecorator::Tick(Blackboard* bb)
{
    if (!_isChildRunning)
    {
        if (!checkCondition(bb))
        {
            return NodeState::Failure;
        }
    }

    if (_child != nullptr)
    {
        NodeState state = _child->Execute(bb);

        if (state == NodeState::Running)
        {
            _isChildRunning = true;
        }
        else
        {
            _isChildRunning = false;
        }

        return state;
    }
    return NodeState::Failure;
}
