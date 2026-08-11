#pragma once
#include "CompositeNode.h"

class Selector : public CompositeNode
{
	virtual NodeState Tick(Blackboard* bb) override;
};