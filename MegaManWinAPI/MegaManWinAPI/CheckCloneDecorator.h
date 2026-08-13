#pragma once
#include "ConditionDecorator.h"

class CheckCloneDecorator : public ConditionDecorator
{
public:
	virtual bool checkCondition(class Blackboard* bb) override;
	virtual void DrawProperty() override;

private:
	class Actor* _clonePtr = nullptr;
	bool _checkExist = true;
};

