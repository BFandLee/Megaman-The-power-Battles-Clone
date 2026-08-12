#pragma once
#include "ConditionDecorator.h"

class ProbabilityDecorator : public ConditionDecorator
{
public:
	// 생성자에서 확률을 인자로 받습니다. (0 ~ 100)
	ProbabilityDecorator(int chance) : _chance(chance) {}

	virtual bool checkCondition(class Blackboard* bb) override;

private:
	int _chance;
};
