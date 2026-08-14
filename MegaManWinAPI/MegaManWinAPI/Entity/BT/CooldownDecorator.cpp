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
    ImGui::PushItemWidth(150.0f);
    ImGui::InputFloat("Cooldown (sec)", &_cooldownTime);
    ImGui::PopItemWidth();
}

void CooldownDecorator::SaveProperty(json& j)
{
    j["CooldownTime"] = _cooldownTime;
}

void CooldownDecorator::LoadProperty(const json& j)
{
    if (j.contains("CooldownTime"))
    {
        _cooldownTime = j["CooldownTime"];
    }
}

