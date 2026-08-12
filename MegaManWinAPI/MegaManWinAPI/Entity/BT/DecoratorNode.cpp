#include "pch.h"
#include "DecoratorNode.h"
#include "Blackboard.h"

NodeState DecoratorNode::Tick(Blackboard* bb)
{
    if (_child != nullptr)
    {
        return _child->Tick(bb);
    }
    return NodeState::Failure;
}
