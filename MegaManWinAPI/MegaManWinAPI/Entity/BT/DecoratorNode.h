#pragma once
#include "BTNode.h"
class DecoratorNode : public BTNode
{
public:
	virtual ~DecoratorNode() { if (_child) delete _child; }
	virtual NodeState Tick(class Blackboard* bb) override;
	void SetChild(BTNode* child) { _child = child; }
	
	virtual void ClearChildren() override
	{
		_child = nullptr;
	}
protected:
	class BTNode* _child = nullptr;
};

