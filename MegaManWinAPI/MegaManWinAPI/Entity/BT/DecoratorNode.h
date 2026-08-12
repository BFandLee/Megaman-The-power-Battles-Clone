#pragma once
#include "BTNode.h"
class DecoratorNode : public BTNode
{
public:
	virtual NodeState Tick(class Blackboard* bb) override;
	void SetChild(BTNode* child) { _child = child; }
protected:
	class BTNode* _child = nullptr;
};

