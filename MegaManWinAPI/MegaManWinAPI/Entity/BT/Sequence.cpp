#include "pch.h"
#include "Sequence.h"

NodeState Sequence::Tick(Blackboard* bb)
{
    for (auto& children : _children)
    {
        switch (children->Tick(bb))
        {
        case NodeState::Success:
            continue;
        
        case NodeState::Failure:
            return NodeState::Failure;
        
        case NodeState::Running:
            return NodeState::Running;
        
        default:
            break;
        }
    }
    return NodeState::Success;
}
