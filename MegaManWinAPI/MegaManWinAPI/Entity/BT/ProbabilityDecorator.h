#pragma once
#include "ConditionDecorator.h"

class ProbabilityDecorator : public ConditionDecorator
{
public:
	virtual bool checkCondition(class Blackboard* bb) override;
	virtual void DrawProperty() override;
private:
	float _probability;
	
};
