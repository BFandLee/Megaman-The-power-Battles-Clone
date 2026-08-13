#include "pch.h"
#include "CooldownDecorator.h"
#include "Blackboard.h"
#include "TimeManager.h"

bool CooldownDecorator::checkCondition(Blackboard* bb)
{
    if ((TimeManager::GetInstance().GetGlobalTime() - _lastExecutionTime) >= _cooldownTime)
    {
        _lastExecutionTime = TimeManager::GetInstance().GetGlobalTime();
        return true;
    }
    return false;
}

void CooldownDecorator::DrawProperty()
{
    ImGui::InputFloat("Cooldown (sec)", &_cooldownTime);
}

