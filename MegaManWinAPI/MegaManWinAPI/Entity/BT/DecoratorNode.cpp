#include "pch.h"
#include "DecoratorNode.h"
#include "Blackboard.h"

NodeState DecoratorNode::Tick(Blackboard* bb)
{
    if (_child != nullptr)
    {
        return _child->Execute(bb);
    }
    return NodeState::Failure;
}

void DecoratorNode::AddChild(BTNode* child)
{
    _child = child;
}
