#pragma once
#include "ConditionDecorator.h"

class CheckCloneDecorator : public ConditionDecorator
{
public:
	CheckCloneDecorator();
	virtual ~CheckCloneDecorator() = default;
	virtual bool checkCondition(class Blackboard* bb) override;
	virtual void DrawProperty() override;
	virtual void SaveProperty(json& j) override;
	virtual void LoadProperty(const json& j) override;

private:
	class Actor* _clonePtr = nullptr;
	bool _checkExist = true;
};

