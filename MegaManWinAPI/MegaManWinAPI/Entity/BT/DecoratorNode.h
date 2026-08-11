#pragma once
#include "BTNode.h"
class DecoratorNode : public BTNode
{
public:
	virtual bool checkCondition() = 0;
	virtual NodeState Tick(class Blackboard* bb) override;
	void SetChild(BTNode* child) { _child = child; }
private:
	class BTNode* _child = nullptr;
};

