#pragma once
#include "ConditionDecorator.h"

class CooldownDecorator : public ConditionDecorator
{
public:
	virtual bool checkCondition(class Blackboard* bb) override;
	virtual void DrawProperty() override;
	virtual void SaveProperty(json& j) override;
	virtual void LoadProperty(const json& j) override;

private:
	float _cooldownTime = 3.0f;
	float _currentTime = 0.0f;
	float _lastExecutionTime = -999.0f;
};
