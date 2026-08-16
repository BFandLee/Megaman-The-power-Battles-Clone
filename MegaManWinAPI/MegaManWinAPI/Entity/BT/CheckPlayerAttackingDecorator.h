#pragma once
#include "ConditionDecorator.h"

class CheckPlayerAttackingDecorator : public ConditionDecorator
{
public:
    CheckPlayerAttackingDecorator() { SetName("CheckPlayerAttacking"); }
    virtual ~CheckPlayerAttackingDecorator() = default;

    // 조건 검사 함수 (BT 실행 시 호출)
    virtual bool checkCondition(class Blackboard* bb) override;

    // BT 에디터(ImGui)용 프로퍼티 드로잉
    virtual void DrawProperty() override;

private:
    bool _checkAttacking = true; // true: 공격 중일 때 실행, false: 미공격 시 실행
};