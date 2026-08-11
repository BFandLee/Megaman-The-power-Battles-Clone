#include "pch.h"
#include "DecoratorNode.h"

NodeState DecoratorNode::Tick(Blackboard* bb)
{
    if (_child != nullptr)
    {
        if (checkCondition())
        {
            return _child->Tick(bb);
        }
        return NodeState::Failure;
    }
    return NodeState::Failure;
    
}
