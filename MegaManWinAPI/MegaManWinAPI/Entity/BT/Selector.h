#pragma once
#include "CompositeNode.h"

class Selector : public CompositeNode
{
public:
	virtual NodeState Tick(Blackboard* bb) override;
};