#pragma once
#include "CompositeNode.h"

class Sequence : public CompositeNode
{
	virtual NodeState Tick(Blackboard* bb) override;
};

