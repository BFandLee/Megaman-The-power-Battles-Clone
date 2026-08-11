#pragma once
#include "BTNode.h"

class ActionNode : public BTNode
{
public:
	virtual NodeState Tick(class Blackboard* bb) override = 0;
	virtual bool IsLeafNode() const { return true; }
};

