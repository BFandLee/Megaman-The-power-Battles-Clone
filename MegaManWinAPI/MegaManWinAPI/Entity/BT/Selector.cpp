#include "pch.h"
#include "Selector.h"

NodeState Selector::Tick(Blackboard* bb)
{
    for (auto& children : _children)
    {
        switch (children->Tick(bb))
        {
        case NodeState::Success:
            return NodeState::Success;

        case NodeState::Failure:
            continue;
        
        case NodeState::Running:
            return NodeState::Running;
        
        default:
            break;
        }
    }
    return NodeState::Failure;
}

