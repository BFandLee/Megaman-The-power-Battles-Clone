#include "pch.h"
#include "Selector.h"

NodeState Selector::Tick(Blackboard* bb)
{
    for (int i = _currentNodeIndex; i < _children.size(); ++i)
    {
        switch (_children[i]->Tick(bb))
        {
        case NodeState::Success:
            _currentNodeIndex = 0;
            return NodeState::Success;

        case NodeState::Failure:
            _currentNodeIndex++;
            continue;
        
        case NodeState::Running:
            _currentNodeIndex = i;
            return NodeState::Running;
        
        default:
            break;
        }
    }
    _currentNodeIndex = 0;
    return NodeState::Failure;
}

