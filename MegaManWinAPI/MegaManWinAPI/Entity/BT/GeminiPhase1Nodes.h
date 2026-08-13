#pragma once
#include "ActionNode.h"


// 분신 활성화 (분신 Actor Spawn)
class BTAction_Gemini_CloneActivate : public ActionNode
{
public:
    BTAction_Gemini_CloneActivate() { SetName("BTAction_Gemini_CloneActivate"); }
    virtual ~BTAction_Gemini_CloneActivate() = default;

    virtual NodeState Tick(class Blackboard* bb) override;
};
