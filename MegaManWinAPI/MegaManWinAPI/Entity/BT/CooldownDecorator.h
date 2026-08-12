#pragma once
#include "ConditionDecorator.h"

class CooldownDecorator : public ConditionDecorator
{
public:
	// 생성자에서 쿨다운 시간을 설정받습니다.
	CooldownDecorator(float cooldownTime) : _cooldownTime(cooldownTime), _timer(cooldownTime) {}

	virtual bool checkCondition(class Blackboard* bb) override;

private:
	float _cooldownTime; // 설정된 쿨다운 시간
	float _timer;        // 현재 흐른 시간 (TODO: Time 매니저 등을 활용하여 업데이트 로직을 고민해보세요)
};
