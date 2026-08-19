#include "pch.h"
#include "Selector.h"

NodeState Selector::Tick(Blackboard* bb)
{
    for (size_t i = _currentNodeIndex; i < _children.size(); ++i)
    {
        if (_children[i] == nullptr)
        {
            continue;
        }

        switch (_children[i]->Execute(bb))
        {
        case NodeState::Success:
            _currentNodeIndex = 0;
            return NodeState::Success;

        case NodeState::Failure:
            _currentNodeIndex++;
            continue;
        
        case NodeState::Running:
            _currentNodeIndex = (int)i;
            return NodeState::Running;
        
        default:
            break;
        }
    }
    _currentNodeIndex = 0;
    return NodeState::Failure;
}

