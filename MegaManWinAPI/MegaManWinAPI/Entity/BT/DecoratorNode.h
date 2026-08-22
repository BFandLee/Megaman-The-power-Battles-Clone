#pragma once
#include "BTNode.h"
class DecoratorNode : public BTNode
{
public:
	virtual ~DecoratorNode() { if (_child) delete _child; }
	virtual NodeState Tick(class Blackboard* bb) override;
	virtual void AddChild(BTNode* child) override;
	
	virtual void ClearChildren() override
	{
		_child = nullptr;
	}
protected:
	class BTNode* _child = nullptr;
};

