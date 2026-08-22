#pragma once
#include "DecoratorNode.h"

class ConditionDecorator : public DecoratorNode
{
public:
	// 매개변수로 Blackboard를 받도록 수정됨
	virtual bool checkCondition(class Blackboard* bb) = 0;

	virtual NodeState Tick(class Blackboard* bb) override;

private:
	bool _isChildRunning = false;
};
