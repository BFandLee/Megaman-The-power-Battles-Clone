#pragma once
#include "CompositeNode.h"

class Sequence : public CompositeNode
{
public:
	virtual NodeState Tick(class Blackboard* bb) override;
};

