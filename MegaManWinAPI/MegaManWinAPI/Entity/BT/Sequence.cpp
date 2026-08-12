#include "pch.h"
#include "Sequence.h"

NodeState Sequence::Tick(Blackboard* bb)
{
    for (int i= _currentNodeIndex; i< _children.size(); ++i)
    {
        switch (_children[i]->Tick(bb))
        {
        case NodeState::Success:
            _currentNodeIndex++;
            continue;
        
        case NodeState::Failure:
            _currentNodeIndex = 0;
            return NodeState::Failure;
        
        case NodeState::Running:
            _currentNodeIndex = i;
            return NodeState::Running;
        
        default:
            break;
        }
    }
    _currentNodeIndex = 0;
    return NodeState::Success;
}
