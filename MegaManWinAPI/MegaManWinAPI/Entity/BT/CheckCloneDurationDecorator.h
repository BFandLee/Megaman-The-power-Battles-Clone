#pragma once
#include "ConditionDecorator.h"

class CheckCloneDurationDecorator : public ConditionDecorator
{
public:
    CheckCloneDurationDecorator();
    virtual ~CheckCloneDurationDecorator() = default;

    virtual bool checkCondition(class Blackboard* bb) override;
    virtual void DrawProperty() override;
    virtual void SaveProperty(json& j) override;
    virtual void LoadProperty(const json& j) override;

private:
    float _minDuration = 5.0f; // 기본 최소 유지 시간 (초)
    class Actor* _clonePtr = nullptr;
};