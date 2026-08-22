#pragma once
#include "ConditionDecorator.h"

class ProbabilityDecorator : public ConditionDecorator
{
public:
	virtual bool checkCondition(class Blackboard* bb) override;
	virtual void DrawProperty() override;
	virtual void SaveProperty(json& j) override;
	virtual void LoadProperty(const json& j) override;

private:
	float _probability;
	
};
