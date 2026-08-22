#pragma once
#include "DecoratorNode.h"

class SucceederDecorator : public DecoratorNode
{
public:
    virtual NodeState Tick(class Blackboard* bb) override
    {
        if (_child == nullptr)
            return NodeState::Success;

        NodeState state = _child->Execute(bb);

        // TODO: 만약 state가 Running이면 Running을 반환하고,
        //       state가 Failure이거나 Success이면 무조건 Success를 반환하도록 작성해보세요!

        if (state == NodeState::Running)
        {
            return NodeState::Running;
        }
        else if (state == NodeState::Success || state == NodeState::Failure)
        {
            return NodeState::Success;
        }

        return state;
    }
};